#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <sstream>
#include <unistd.h>

#include "Sender.h"


Sender::Sender(std::string protocol, std::string host)
{
    _protocol = protocol;
    _host = host;
    
    // Преобразование имени хоста
    struct hostent* raw_host;
    raw_host = gethostbyname(_host.c_str());
    if (raw_host == NULL)
    {
        std::cerr << "Ошибка, данный хост не существует или некорректен" << std::endl;
        exit(0);
    }

    // Создаем сокет с доменом Internet и типом SOCK_STREAM (с предварительной установкой соединения).
    _socket = socket(AF_INET, SOCK_STREAM, 0);

    // Настраиваем адрес
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    bcopy((char*) raw_host->h_addr, (char*) &addr.sin_addr, raw_host->h_length);

    // Установка соединения
    if (connect(_socket, (struct sockaddr *) &addr, sizeof (addr)) < 0)
    {
        std::cerr << "Не удалось установить соединение." << std::endl;
        exit(2);
    }
}

Sender::~Sender()
{
    if(close(_socket) != 0)
    {
        std::cerr << "Не удалось закрыть сокет." << std::endl;
        exit(2);
    }
}

std::string Sender::SendGET(std::string path, std::vector< std::pair< std::string, std::string> > params)
{
    std::string GETParamsString = ConvertParams(params);
    
    std::string header = "";
    
    header += "GET ";
    header += _protocol + "://" +  _host + "/" + path + "?" + GETParamsString + "\r\n";
    header += (std::string)" HTTP/1.1" + "\r\n";
    header += (std::string)"Host: " + _protocol + "://" + _host + "/" + "\r\n";
    header += (std::string)"User-Agent: Mozilla/5.0" + "\r\n";
    //header += (std::string)"Accept: text/html" + "\r\n";
    header += (std::string)"Accept-Language: ru,en-us;q=0.7,en;q=0.3" + "\r\n";
    header += (std::string)"Accept-Charset: windows-1251,utf-8;q=0.7,*;q=0.7" + "\r\n";
    header += (std::string)"Connection: keep-alive " + "\r\n";
    header += "\r\n";

    return Send(header.c_str(), header.size(), true);
}

std::string Sender::Send(const char * header, size_t length, bool needAnswer)
{
    // Отправляем запрос.
    send(_socket, header, length, 0);

    if(!needAnswer)
        return NULL;

    char buf[1024];
    std::string result = "";

    // Принимаем ответ, но по частям
    int receivedBytes = recv(_socket, buf, sizeof(buf), 0);
    while(receivedBytes != 0)
    {
        if(receivedBytes < 0)
        {
            std::cerr << "Ошибка при получении данных с сервера." << std::endl;
            exit(2);
        }
        
        result.append(buf, receivedBytes); 

        receivedBytes = recv(_socket, buf, sizeof(buf), 0);
    }

    return result;
}


std::string Sender::ConvertParams(std::vector< std::pair< std::string, std::string> > params) {
    std::string parm = "";

    for (int i = 0; i < params.size(); ++i) {
        if (i != 0)
            parm += "&";

        parm += params[i].first + "=" + params[i].second;
    }

    return parm;
}

