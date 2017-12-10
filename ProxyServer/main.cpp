#include <cstdlib>

#include "../ProxyServer.Modules/Server/Server.h"

#define CONFIG_FILE "/home/user/DDZ/MP_DDZ/ConfigFile"

Server* ptr;

void TurnOffServer(int signal)
{
    
    if (signal == SIGINT)
    {
        ptr->Stop();
        std::cout << "\nОстановка сервера !!!\n";
    }
    
    if (signal == SIGUSR1)
    {
        ptr->UpdateConfig();
        std::cout << "\nКонфиг обновлен\n";
    }
}

int main(int argc, char** argv)
{
    signal(SIGINT, TurnOffServer);
    signal(SIGUSR1, TurnOffServer);
    
    ptr = new Server(CONFIG_FILE);
    ptr->Start();
    
    return 0;
}