#include "Server.h"

bool turn = true;  

template <typename T>
std::string ToString(T val)
{
    std::stringstream ss;
    ss << val;
    int temp;
    ss >> temp;
    return temp;
}

void Server::FindHostAndPort(std::string request, std::string& host, std::string& port)
{
    // Ищем конец строки, в которой записан протокол
    size_t pos = request.find("Host: ");
    size_t endHost = request.find(":", pos + 6);
    size_t endPort = request.find( "\r\n", pos + 6);
    
    if(endPort > endHost)
        port = request.substr(endHost + 1, endPort - endHost - 1);
    else
    {
        endHost = endPort;
        port = "80";
    }
    
    host = request.substr(pos + 6, endHost - pos - 6);
}

Server::Server(const char* config)
{
    // Загружаем конфигурационный файл
    _configuration = Configurator::Read(config);

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
    listen(_listener, 1);

    // Инициализируем кешер с настройками, заданными в конфигурационном файле
    _cacher = new Cacher("./cachefiles/", _configuration.CacheSize, _configuration.CacheRelevanceTime);

    _listening = false;
    _theadsCount = 5;
}
    
Server::~Server()
{
    delete _cacher;
}

void Server::Start()
{
    char buf[2050]; // Буфер для считывания запросов
    
    _listening = true;
    
    while(_listening)
    {
        // Получаем дискриптор клиентского сокета
        int sock = accept(_listener, NULL, NULL);
        
        // Если флаг работы сервера сброшен, то выходим
        if(!_listening)
            break;
        
        // Если дескриптор не получен, то выходим с ошибкой
        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }

        // Читаем данные из сокета
        int receivedBytes = recv(sock, buf, sizeof(buf), 0);
        
        // Если из сокета не получено данных, то переходим к ожиданию следующего запроса
        if (receivedBytes < 0)
        {
            perror("socket read");
            continue;
        }
        
        std::string request(buf);
        
        
        // Парсим адрес хоста
        std::string host;
        std::string port;
        FindHostAndPort(request, host, port); 
        
        // _DO_ Проверка по ключу - переделать функцию выше, чтобы она находила и параметры.
        
        // Проверяем в кэше
        std::string answer = _cacher->Get(request);
        
        // Если найдено в кеше, то сразу отправляем ответ
        if(answer != "")
        {
            std::cout << std::endl << "Найдено в кэше";
            
            send(sock, answer.c_str(), answer.length(), 0); 
            
            close(sock);
            
            continue;
        }
                       
        // Создаем объект с помощью которого отсылается запрос на сервер
        Sender sender("http", host, ToInt(port));
        
        std::cout << "Отправляю запрос" << "\n";
        
        // Посылаем запрос на сервер и получаем ответ
        answer = sender.Send(request.c_str(), request.length(), true);
        
        std::cout << "Отправляю ответ размера " <<  answer.length() << "\n";
        
        // Если по запросу найден ответ, то кешируем его
        if (IsGoodAnswer(answer))
            _cacher->Put(request, answer);
        
        // Отправляем ответ пользователю
        send(sock, answer.c_str(), answer.length(), 0);
        
        // Закрываем сокет
        close(sock);
    }
}

void Server::Stop()
{
    _listening = false;
    close(_listener);
}

void Server::UpdateConfig()
{
    
}



bool Server::IsGoodAnswer(std::string answer)
{
    // Находим конец строки
    size_t end = answer.find("\r\n");
    
    // Получаем из ответа перувую строку
    std::string first_str = answer.substr(0, end + 2); 
     
    return first_str.find("200 OK") != std::string::npos;
}