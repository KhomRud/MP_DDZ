#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <unistd.h>
#include <string>

#include "../ProxyServer.Modules/Sender/Sender.h"

#define HOST "Host: 192.168.101.129:80"

std::string CreateRequest(char buf[])
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

    return protocol + HOST + str_buf;
}

int main()
{
    int sock; // Идентификатор клиентского сокета
    int listener; // Идентификатор принимающего сокета
    struct sockaddr_in addr; // Адрес сокета
    char buf[1024]; // Буфер для считывания запросов
    std::string answer; // Ответ от сервера
    
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
    
    while(1)
    {
        // Получаем дискриптор клиентского сокета
        sock = accept(listener, NULL, NULL);
        
        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }
        
        // Читаем данные из сокета
        recv(sock, buf, 1024, 0);
        
        // Создаем запрос для сервера
        std::string request = CreateRequest(buf);
        
        // Создаем объект с помощью которого отсылается запрос на сервер
        Sender sender("http", "192.168.101.129");
        
        // Посылаем запрос на сервер и получаем ответ
        answer = sender.Send(request.c_str(), request.length(), true);
        
        // Отправляем ответ пользователю
        send(sock, answer.c_str(), answer.length(), 0);
        
        // Закрываем сокет
        close(sock);
    }
    
    return 0;
}