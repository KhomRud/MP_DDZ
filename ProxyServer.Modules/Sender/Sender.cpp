#include "Sender.h"

Sender::Sender(std::string protocol, std::string host, int port)
{
    _protocol = protocol;
    _host = host;
    
    // Преобразование имени хоста
    addrinfo* host_info;
    if (getaddrinfo(_host.c_str(), _protocol.c_str(), 0, &host_info) != 0)
    {
        perror("Данный хост не существует или некорректен");
        return;
    }

    // Создаем сокет с доменом Internet и типом SOCK_STREAM (с предварительной установкой соединения).
    _socket = socket(AF_INET, SOCK_STREAM, 0);

    // Установка соединения
    if (connect(_socket, host_info->ai_addr, host_info->ai_addrlen) < 0)
    {
        perror("Не удалось установить соединение");
        return;
    }
}

Sender::~Sender()
{
    if(close(_socket) != 0)
    {
        perror("Не удалось закрыть сокет");
        exit(4);
    }
}

std::string Sender::Send(const char * header, size_t length, bool needAnswer)
{
    // Отправляем запрос.
    send(_socket, header, length, 0);

    if(!needAnswer)
        return NULL;

    char buf[2050];
    std::string result = "";

    // Принимаем ответ, но по частям
    int receivedBytes = recv(_socket, buf, sizeof(buf), 0);
    while(receivedBytes != 0)
    {
        if(receivedBytes < 0)
        {
            perror("Ошибка при получении прокси-сервером данных с хоста");
            return result;
        }
        
        result.append(buf, receivedBytes); 

        if(receivedBytes < sizeof(buf))
            break;
        
        receivedBytes = recv(_socket, buf, sizeof(buf), 0);
    }

    return result;
}