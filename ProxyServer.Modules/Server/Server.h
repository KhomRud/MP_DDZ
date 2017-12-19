#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <unistd.h>
#include <string>
#include <pthread.h>
#include <csignal>
#include <utility>
#include <vector>
#include <fcntl.h>

#include "../Configurator/Configurator.h"
#include "../Cacher/Cacher.h"
#include "../Sender/Sender.h"

#ifndef SERVER_H
#define SERVER_H

template <typename T>
int ToInt(T val)
{
    std::stringstream ss;
    ss << val;
    int temp;
    ss >> temp;
    return temp;
}

class Server
{   
public:
    /**
     * @param config    Путь до конфигурационного файла           .
     */
    Server(const char* config);
    ~Server();
    
    /**
     * Запуск сервера
     */
    void Start();    
        
    /**
     * Остановка сервера
     */
    void Stop();
    
    /**
     * Перезагрузить конфиг файл
     */
    void UpdateConfig();

private:
    
    int _listener; // Идентификатор принимающего сокета
    bool _listening; // флаг работы сервера
    const char* _config; // Путь до конфига
    
    ConfigurationData _configuration; // Текущая конфигурация
    Cacher* _cacher;    // Объект кешера
    std::list<pthread_t*> *_threads;  // Лист, хранящий идентификаторы потоков 
};


#endif /* SERVER_H */

