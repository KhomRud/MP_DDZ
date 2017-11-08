#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <unistd.h>

int main()
{
    int sock; // Идентификатор клиентского сокета
    int listener; // Идентификатор принимающего сокета
    struct sockaddr_in addr; // Адрес сокета
    char buf[1024]; // Буфер для считывания запросов
    int bytes_read; // Количество считаных байтов
    
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
        bytes_read = recv(sock, buf, 1024, 0);
        
        for (int i = 0; i < 1024; ++i)
            printf("%c", buf[i]);
        
        // Закрываем сокет
        close(sock);
    }
    
    return 0;
}