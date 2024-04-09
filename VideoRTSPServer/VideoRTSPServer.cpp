#include <iostream>
#include <fstream>
#include "RTSPServer.h"
#include "Socket.h"
#include "CThreadPool.h"
#include "RTPHelper.h"
int main()
{
    std::string filename = MediaPath;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "MediaPath文件不存在！" << std::endl;
        return 0;
    }
    RTSPServer server;
    server.Init();
    server.Invoke();
    std::cout << "press any key to stop..." << std::endl;
    getchar();

    return 0;
}



