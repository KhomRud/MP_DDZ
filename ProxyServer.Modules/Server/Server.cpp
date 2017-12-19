#include <asm-generic/errno-base.h>

#include "Server.h"

#define THREAD_COUNT 10

bool turn = true; 

struct ThreadData
{
    int Sock;
    Cacher* CacherObject;
    
    std::list<pthread_t*>* Threads;
    pthread_t* Id;
};

struct RequestParts
{
    std::string Host;
    std::string Port;
    std::string Path;
    std::string Params;
};

bool IsGoodAnswer(std::string answer)
{
    // Находим конец строки
    size_t end = answer.find("\r\n");
    
    // Получаем из ответа перувую строку
    std::string first_str = answer.substr(0, end + 2); 
     
    return first_str.find("200 OK") != std::string::npos;
}

std::string GetParams(std::string request)
{
    size_t begin;
    size_t end;
    
    std::string params("");
    
    if (request.substr(0, 3) == "GET")
    {
        end = request.find("\r\n");
        
        params = request.substr(0, end);
        
        begin = params.find("?");
        
        if (begin == std::string::npos)
            return "";
        
        end = params.find_last_of("HTTP");
        
        params = params.substr(begin + 1, end - begin - 4);
    }
    else
    {
        begin = request.find("\r\n\r\n");
        
        if (begin == std::string::npos)
            return params;
        
        params = request.substr(begin + 4, request.length() - begin - 4);
    }
    
    return params;
}

std::string GetPath(std::string request)
{
    size_t begin;
    size_t end;
    
    std::string path("");
    
    begin = request.find("http://");
    end = request.find("\r\n");

    begin += 7;

    path = request.substr(begin, end - begin);

    begin = path.find("/");
    end = path.find_last_of("HTTP");

    ++begin;    

    path = path.substr(begin, end - begin - 4);
    
    if (request.substr(0, 3) == "GET")
    {
        end = path.find("?");
        
        if (end == std::string::npos)
            return path;
        
        path = path.substr(0, end);
    }
    
    return path;
}
    
    /**
     * Распарсить запрос
     * @param request    Запрос
     * @param data    Возвращаемая структура с частями запроса         .
     */
void ParseRequest(std::string request, RequestParts& data)
{
    data.Params = GetParams(request);
    data.Path = GetPath(request);
        
    // Ищем конец строки, в которой записан протокол
    size_t pos = request.find("Host: ");
    size_t endHost = request.find(":", pos + 6);
    size_t endPort = request.find( "\r\n", pos + 6);
    
    if(endPort > endHost)
        data.Port = request.substr(endHost + 1, endPort - endHost - 1);
    else
    {
        endHost = endPort;
        data.Port = "80";
    }
    
    data.Host = request.substr(pos + 6, endHost - pos - 6);
}

Server::Server(const char* config)
{
    // Загружаем конфигурационный файл
    _config = config;
    _configuration = Configurator::Read(_config);

    // Адрес сокета
    struct sockaddr_in addr;

    // Получаем идентификатор сокета
    _listener = socket(AF_INET, SOCK_STREAM, 0);
  
    if (_listener < 0)
    {
        perror("socket");
        exit(1);
    }

    // Задаем семейство адресов
    addr.sin_family = AF_INET;
    // Задаем порт
    addr.sin_port = htons(_configuration.Port);
    // Задаем принимаемые ip адреса
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Связываем сокет с адресом
    if (bind(_listener, (struct sockaddr *) &addr, sizeof (addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    // Переводим сокет в режим ожидания запросов
    listen(_listener, THREAD_COUNT);

    // Инициализируем кешер с настройками, заданными в конфигурационном файле
    _cacher = new Cacher("./cachefiles/", _configuration.CacheSize, _configuration.CacheRelevanceTime);

    _listening = false;
    
    _threads = new std::list<pthread_t*>;
}
    
Server::~Server()
{
    delete _cacher;
}

void ThreadExit(ThreadData* data)
{
    data->Threads->remove(data->Id);
            
    if(close(data->Sock) != 0)
        perror("Не удалось закрыть сокет, принимающий запрос клиента.");
    
    delete data;
    
    pthread_exit(0);
}

void* ThreadProcessing(void* threadData)
{
    ThreadData* data  = (ThreadData*) threadData;
    
    std::string request = "";
    char buf[2048]; // Буфер для считывания запросов

    // Читаем данные из сокета, но по частям
    int receivedBytes = recv(data->Sock, buf, sizeof(buf), 0);
    while(receivedBytes != 0)
    {
        if(receivedBytes < 0)
        {
            perror("Ошибка при получении данных запроса");
            break;
        }

        request.append(buf, receivedBytes); 

        if(receivedBytes < sizeof(buf))
            break;

        receivedBytes = recv(data->Sock, buf, sizeof(buf), 0);
    }

    if(request == "")
    {
        std::cout << "Пустой запрос! \n";
        ThreadExit(data);
    }

    if (request.substr(0, 3) != "GET" && request.substr(0, 4) != "POST")
    {
        std::cout << "Необрабатываемый запрос! \n";
        ThreadExit(data);
    }

    // Парсим адрес хоста
    RequestParts parts;
    ParseRequest(request, parts); 

    std::string head = request.substr(0, request.find("\r\n"));
    std::cout << "Head of request: " << head << std::endl;

    std::string url = parts.Host + ":"+ parts.Port + "/" + parts.Path + "?" + parts.Params;
    std::cout << "Parsed url: " << url << std::endl;

    std::string answer = "";

    // Проверяем в кэше
    answer = data->CacherObject->Get(url);

    // Если найдено в кеше, то сразу отправляем ответ
    if(answer != "")
    {
        std::cout << "Найдено в кэше. \n";

        send(data->Sock, answer.c_str(), answer.length(), 0); 

        ThreadExit(data);
    }

    std::cout << "Устанавливаю соединение" << "\n";

    // Создаем объект с помощью которого отсылается запрос на сервер
    Sender sender("http", parts.Host, ToInt(parts.Port));
    
    std::cout << "Отправляю запрос" << "\n";

    // Посылаем запрос на сервер и получаем ответ
    answer = sender.Send(request.c_str(), request.length(), true);

    std::cout << "Отправляю ответ размера " <<  answer.length() << "\n";

    // Если по запросу найден ответ, то кешируем его
    if (IsGoodAnswer(answer))
        data->CacherObject->Put(url, answer);

    // Отправляем ответ пользователю
    send(data->Sock, answer.c_str(), answer.length(), 0);

    std::cout << "-------------------------------------" << "\n"<< "\n";
    
    // Закрываем сокет
    ThreadExit(data);
}

void Server::Start()
{
    _listening = true;
    
    while(_listening)
    {
        // Получаем дискриптор клиентского сокета
        int sock = accept(_listener, NULL, NULL);
        
        while(_threads->size() == THREAD_COUNT)
            sleep(1);
            
        // Если флаг работы сервера сброшен, то выходим
        if(!_listening)
            break;
        
        // Если дескриптор не получен, то выходим с ошибкой
        if(sock < 0)
        {
            perror("Accept Error!");
            continue;
        }
        
        // Подготовка нового потока       
        pthread_t* thread = new pthread_t;
        
        ThreadData* data = new ThreadData;
        data->Sock = sock;
        data->CacherObject = _cacher;
        data->Threads = _threads;
        data->Id = thread;
        
        pthread_create(thread, NULL, ThreadProcessing, (void *) data);
    
        // Запись информации о новом потоке
        _threads->push_back(thread);
    }
}

void Server::Stop()
{
    // Отключение порта-сервера
    close(_listener);
    
    // Остановка запущенных потоков
    for(std::list<pthread_t*>::iterator it = _threads->begin(); it != _threads->end(); ++it)
        pthread_cancel(**it);
}

void Server::UpdateConfig()
{
    // Обновление конфиг-файла
    _configuration = Configurator::Read(_config);
    _cacher->Resize(_configuration.CacheSize);
}

