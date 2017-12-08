/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: user
 *
 * Created on December 7, 2017, 11:00 AM
 */

#include <cstdlib>

#include "../ProxyServer.Modules/Server/Server.h"

#define CONFIG_FILE "/home/user/DDZ/MP_DDZ/ConfigFile"

int main(int argc, char** argv)
{
    Server server(CONFIG_FILE);
    server.Start();
    
    return 0;
}

