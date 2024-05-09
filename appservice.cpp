#include "appservice.h"
#include "net/udpserver.h"
#include "net/tcpserver.h"
#include "net/tcpsocket.h"
#include "conf/appconfig.h"

#include <thread>
#include <iostream>

using namespace std;

AppService::AppService()
{

}

AppService *AppService::getInstance()
{
    static AppService appSignal;
    return &appSignal;
}

void AppService::start()
{
    string address = AppConfig::getInstance()->getValue("App", "address");
    string port = AppConfig::getInstance()->getValue("App", "port");
    if (address.empty() || port.empty())
    {
        LOG_DEBUG("the server conf value is empty, exit.");
        return;
    }

    string mode = AppConfig::getInstance()->getValue("App", "mode");
    if (mode.empty()) mode = "0";
    if (stoi(mode) == 0)
    {
        UdpServer server(address, stoi(port));
        server.start();

        while (true) std::this_thread::sleep_for(std::chrono::seconds(1800));
    }
    else if (stoi(mode) == 1)
    {
        TcpServer server(address, stoi(port));
        server.start();

        while (true) std::this_thread::sleep_for(std::chrono::seconds(1800));
    }
    else
    {
        TcpSocket socket(address, stoi(port));
        socket.start();

        while (true) std::this_thread::sleep_for(std::chrono::seconds(1800));
    }

    LOG_DEBUG("service end this time.");
}
