#include <iostream>
#include "RTSPServer.h"
#include "Socket.h"
#include "CThreadPool.h"
#include "RTPHelper.h"


int main()
{
    RTSPServer server;
    server.Init();
    server.Invoke();
    std::cout << "press any key to stop..." << std::endl;
    getchar();

    return 0;
}



