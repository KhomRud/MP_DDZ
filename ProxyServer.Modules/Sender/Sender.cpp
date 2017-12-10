#include <fcntl.h>
#include <asm-generic/errno-base.h>

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

    // Создаем сокет с доменом Internet и типом SOCK_STREAM (с предварительной установкой соединения)
    _socket = socket(AF_INET, SOCK_STREAM, 0);

    // Установка соединения
    if (connect(_socket, host_info->ai_addr, host_info->ai_addrlen) < 0)
    {
        perror("Не удалось установить соединение");
        return;
    }
    
    // Устанавливаем сокет неблокирующим для удобства считывания
    fcntl(_socket, O_NONBLOCK);
}

Sender::~Sender()
{
    if(close(_socket) != 0)
        perror("Не удалось закрыть сокет");
}

std::string Sender::Send(const char * header, size_t length, bool needAnswer)
{
    // Отправляем запрос.
    send(_socket, header, length, 0);

    // Если ответ не нужен, то не ждем его
    if(!needAnswer)
        return "";

    char buf[2000];
    std::string result = ""; 
    
    // Ждем, пока данные придут на сокет
    int receivedBytes = EAGAIN;
    
    while(receivedBytes == EAGAIN)
        receivedBytes = recv(_socket, buf, sizeof(buf), 0);
    
    // Принимаем ответ, но по частям
    while(receivedBytes != 0)
    {
        if(receivedBytes < 0)
        {
            perror("Ошибка при получении прокси-сервером данных с хоста");
            return result;
        }
        
        result.append(buf, receivedBytes); 
        receivedBytes = recv(_socket, buf, sizeof(buf), MSG_WAITALL);
    }

    return result;
}