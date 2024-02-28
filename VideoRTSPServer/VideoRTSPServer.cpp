#include <iostream>
#include "RTSPServer.h"
#include "Socket.h"
#include "CThreadPool.h"


int main()
{
    RTSPServer server;
    server.Init();
    server.Invoke();
    getchar();
    server.Stop();

    return 0;
}



