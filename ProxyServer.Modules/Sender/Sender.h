#pragma once

#include <string>
#include <vector>

#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>

#ifndef SENDER_H
#define SENDER_H



/** 
 * Устанавливает соединение с хостом по протоколу и отправляет сообщения с возможностью получения 
 */
class Sender {
public:
    
    /**
     * @param protocol  Протокол, по которому класс будет связываться с хостом
     * @param host      Адрес хоста
     * @param host      Порт хоста
     */
    Sender(std::string protocol, std::string host, int port);
    ~Sender();

    /**
     * Оправить произвольный запрос на хост.
     * @param header        Строка, содержащая запрос
     * @param length        Длина запроса
     * @param needAnswer    Необходимость ожидания ответа (по умолчанию true)
     * @return              Ответ сервера
     */
    std::string Send(const char * header, size_t length, bool needAnswer = true);

private:
    int _socket; // Идентификатор сокета
    std::string _protocol; // Название протокола, по которому идёт соединение
    std::string _host; // Имя хоста
};

#endif /* SENDER_H */ 