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

struct ThreadData
{
    int Sock;
    std::string Request;
    std::string Host;
    std::string Port;
    
    Cacher* CacherObject;
    bool* Listening;
    int Listener;
};

struct MainThreadData
{

};

class Server
{   
public:
    /**
     * 
     * @param cacheFolder           Директория для записи ответов.
     */
    Server(const char* config);
    ~Server();
    
    
    void Start();
    
   void FindHostAndPort(std::string str, std::string& host, std::string& port);
    
    bool static IsGoodAnswer(std::string answer);
    void TurnOffServer(int signal);
    
private:
    
    int _listener; // Идентификатор принимающего сокета
    bool _listening;
    int _theadsCount;
    Cacher* _cacher;
    
    
    pthread_t _mainThread;
};


#endif /* SERVER_H */

