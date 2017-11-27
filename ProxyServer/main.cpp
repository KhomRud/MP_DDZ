#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <unistd.h>
#include <string>

#include "../ProxyServer.Modules/Cacher/Cacher.h"
#include "../ProxyServer.Modules/Sender/Sender.h"
#include "../ProxyServer.Modules/Configurator/Configurator.h"

#define CONFIG_FILE "/home/user/DDZ/MP_DDZ/ConfigFile"

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

bool IsBadAnswer(std::string answer)
{
    size_t end = answer.find("\r\n");
    
    std::string first_str = answer.substr(0, end + 2); 
     
    return (first_str.find("Not Found") != std::string::npos) &&
            (first_str.find("Bad Request") != std::string::npos);
}

int main()
{
    int sock; // Идентификатор клиентского сокета
    int listener; // Идентификатор принимающего сокета
    struct sockaddr_in addr; // Адрес сокета
    char buf[1024]; // Буфер для считывания запросов
    std::string answer; // Ответ от сервера
    
    // Загружаем онфигурационный файл
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
    
    Cacher cacher("./cachefiles/", configuration.CacheSize, configuration.CacheRelevanceTime);
    
    while(1)
    {
        // Получаем дискриптор клиентского сокета
        sock = accept(listener, NULL, NULL);
        
        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }
        
        std::string str_buf = "";

        // Читаем данные из сокета
        int receivedBytes = recv(sock, buf, sizeof(buf), 0);
        
        if (receivedBytes < 0)
        {
            perror("socket read");
            exit(5);
        }
        
        // Проверяем в кэше
        answer = cacher.Get(str_buf);
        
        if(answer != "")
        {
            std::cout << std::endl << "Найдено в кэше";
            send(sock, answer.c_str(), answer.length(), 0);  
            continue;
        }
               
        // Создаем запрос для сервера
        std::string request = CreateRequest(buf, configuration.Host, configuration.Port);
        
        // Создаем объект с помощью которого отсылается запрос на сервер
        Sender sender("http", configuration.Host, configuration.Port);
        
        // Посылаем запрос на сервер и получаем ответ
        answer = sender.Send(request.c_str(), request.length(), true);
        
        cacher.Put(str_buf, answer);
        
        /*
        if (!IsBadAnswer(answer))
            std::cout << std::endl << "good answer" << std::endl;
        else
            std::cout << std::endl << "find bad answer" << std::endl;
        */
        
        // Отправляем ответ пользователю
        send(sock, answer.c_str(), answer.length(), 0);
        
        // Закрываем сокет
        close(sock);
    }
    
    return 0;
}