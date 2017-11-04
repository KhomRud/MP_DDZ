// #include <cstdlib>
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

#include "Sender.h"

Sender::Sender(std::string protocol, std::string host, std::string path, std::vector< std::pair<std::string, std::string> > params) {
    _protocol = protocol;
    _host = host;
    _path = path;
    _params = ConvertParams(params);
}

/*Пока в задумках, надо потщательнее посмотреть варианты. но сейчас несущественно.
Sender(std::string url) {
    _protocol = GetProtocol(url);
    _host = GetHost(url);
    _path = GetPath(url);
    _params = GetPath(url);
}

std::string GetProtocol(std::string url) {
    std::string protocol = "";

    int i = 0;

    for (i = 0; i < url.size(); i++) {
        if (url[i] != '/' || url[i + 1] != '/') {
            protocol += url[i];
        } else {
            protocol += "//";
            break;
        }
    }

    return protocol;
}

std::string GetHost(std::string url) {
    std::string host = "";

    url.replace(0, getProtocol(url).size(), "");

    int i = 0;
    for (i = 0; i < url.size(); i++) {

        if (url[i] != '/') {
            host += url[i];
        } else {
            break;
        }

    }

    return host;
}

std::string GetPath(std::string url) {
    std::string parm = "";

    url.replace(0, getProtocol(url).size() + getHost(url).size(), "");

    int i = 0;
    for (i = 0; i < url.size(); i++) {

        if (url[i] != '?' && url[i] != '#') {
            parm += url[i];
        } else {
            break;
        }

    }

    return parm;
}

 */

std::string Sender::ConvertParams(std::vector< std::pair< std::string, std::string> > params) {
    std::string parm = "";

    for (int i = 0; i < params.size(); ++i) {
        if (i != 0)
            parm += "&";

        parm += params[i].first + "=" + params[i].second;
    }

    return parm;
}

// пока криво, надо снести в конструктор создание сокета, а здесь оставить сборку в запрос

std::string Sender::SendGET() {
    char buf[1024];

    std::string header = "";

    header += "GET ";
    header += _protocol + "://" +  _host + "/" + _path + "?" + _params + "\r\n";
    header += (std::string)" HTTP/1.1" + "\r\n";
    header += (std::string)"Host: " + _protocol + "://" + _host + "/" + "\r\n";
    header += (std::string)"User-Agent: Mozilla/5.0" + "\r\n";
    //header += (std::string)"Accept: text/html" + "\r\n";
    header += (std::string)"Accept-Language: ru,en-us;q=0.7,en;q=0.3" + "\r\n";
    header += (std::string)"Accept-Charset: windows-1251,utf-8;q=0.7,*;q=0.7" + "\r\n";
    header += (std::string)"Connection: keep-alive " + "\r\n";
    header += "\r\n";



    int sock;
    struct hostent* raw_host;
    raw_host = gethostbyname(_host.c_str());
    if (raw_host == NULL) {
        std::cout << "ERROR, no such host";
        exit(0);
    }

    // Создаем сокет с доменом Internet и типом SOCK_STREAM (с предварительной установкой соединения).
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // Настраиваем адрес
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    bcopy((char*) raw_host->h_addr, (char*) &addr.sin_addr, raw_host->h_length);

    // Связывать сокет с адресом - необязательно, connect сама это делает.
    // bind(sock, addr, sizeof(addr));

    // Установка соединения
    if (connect(sock, (struct sockaddr *) &addr, sizeof (addr)) < 0) {
        std::cerr << "connect error" << std::endl;
        exit(2);
    }


    char * message = new char[ header.size() ];
    for (int i = 0; i < header.size(); i++)
        message[i] = header[i];

    // Отправляем запрос.
    send(sock, header.c_str(), header.size(), 0);
    recv(sock, buf, sizeof (buf), 0);

    std::string result = "";

    for (int j = 0; j < 1024; j++) 
    {
        if (buf[j] != '\0')
            result += buf[j];
        else
            break;
    }

    return result;
}
