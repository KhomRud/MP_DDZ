#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <unistd.h>
#include <string>
#include <csignal>

#include "../ProxyServer.Modules/Cacher/Cacher.h"
#include "../ProxyServer.Modules/Sender/Sender.h"
#include "../ProxyServer.Modules/Configurator/Configurator.h"

#define CONFIG_FILE "/home/user/DDZ/MP_DDZ/ConfigFile"

bool turn = true;  
int listener; // Идентификатор принимающего сокета
std::string CreateRequest(char buf[], std::string host, std::string port)
{
    std::string str_buf(buf); // Строка для работы с буфером

    // Ищем конец строки, в которой записан протокол
    size_t end = str_buf.find("\r\n");
    
    // Получаем протокол
    std::string protocol = str_buf.substr(0, end + 2); 

    // Отрезаем от строки протокол
    str_buf = str_buf.substr(end + 2, str_buf.length() - end - 2);

    // Ищем конец строки, в которой записан хост
    end = str_buf.find("\r\n");

    // Отрезаем от строки хост
    str_buf = str_buf.substr(end, str_buf.length() - end);

    return protocol + "Host: " + host + ":" + port + str_buf;
}

bool IsGoodAnswer(std::string answer)
{
    // Находим конец строки
    size_t end = answer.find("\r\n");
    
    // Получаем из ответа перувую строку
    std::string first_str = answer.substr(0, end + 2); 
     
    return first_str.find("200 OK") != std::string::npos;
}

void TurnOffServer(int signal)
{
    if (signal == SIGINT)
    {
        turn = false;
    
        close(listener);
    }
    
    if (signal == SIGUSR1)
    {
        std::cout << " Здесь меняют конфиг" << std::endl;
    }
}
/*
int oldmain()
{
    int sock; // Идентификатор клиентского сокета
    struct sockaddr_in addr; // Адрес сокета
    char buf[1024]; // Буфер для считывания запросов
    std::string answer; // Ответ от сервера
    
    signal(SIGINT, TurnOffServer);
    signal(SIGUSR1, TurnOffServer);
    
    // Загружаем конфигурационный файл
    ConfigurationData configuration = Configurator::Read(CONFIG_FILE);
    
    // Получаем идентификатор сокета
    listener = socket(AF_INET, SOCK_STREAM, 0);
    
    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }
    
    // Задаем семейство адресов
    addr.sin_family = AF_INET;
    // Задаем порт
    addr.sin_port = htons(3425);
    // Задаем принимаемые ip адреса
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    // Связываем сокет с адресом
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    // Переводим сокет в режим ожидания запросов
    listen(listener, 1);

    // Инициализируем кешер с настройками, заданными в конфигурационном файле
    Cacher cacher("./cachefiles/", configuration.CacheSize, configuration.CacheRelevanceTime);
    
    while(1)
    {
        // Получаем дискриптор клиентского сокета
        sock = accept(listener, NULL, NULL);
        
        // Если флаг работы сервера сброшен, то выходим
        if(!turn)
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
        
        std::string str_buf(buf);
        
        // Проверяем в кэше
        answer = cacher.Get(str_buf);
        
        // Если найдено в кеше, то сразу отправляем ответ
        if(answer != "")
        {
            std::cout << std::endl << "Найдено в кэше";
            
            send(sock, answer.c_str(), answer.length(), 0); 
            
            close(sock);
            
            continue;
        }
               
        // Создаем запрос для сервера
        std::string request = CreateRequest(buf, configuration.Host, configuration.Port);
        
        // Создаем объект с помощью которого отсылается запрос на сервер
        Sender sender("http", configuration.Host, configuration.Port);
        
        // Посылаем запрос на сервер и получаем ответ
        answer = sender.Send(request.c_str(), request.length(), true);
        
        // Если по запросу найден ответ, то кешируем его
        if (IsGoodAnswer(answer))
            cacher.Put(str_buf, answer);
        
        // Отправляем ответ пользователю
        send(sock, answer.c_str(), answer.length(), 0);
        
        // Закрываем сокет
        close(sock);
    }
    
    return 0;
}

*/