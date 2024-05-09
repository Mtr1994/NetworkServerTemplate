#include "udpserver.h"
#include "log/logger.h"

#include <iostream>

using namespace  std;

UdpServer::UdpServer(const std::string &address, uint16_t port)
    :mLocalAddress(address), mLocalPort(port)
{
    mUdpServer.Attach(UdpServer_Creator::Create(&mUdpServerListener));
}

void UdpServer::start()
{
    // 初始化消息解析线程
    MessageParser::getInstance()->init();

    if (mUdpServer->GetState() <= SS_STARTED) return;
    bool status = mUdpServer->Start(LPCTSTR(mLocalAddress.data()), mLocalPort);
    LOG_DEBUG(status ? "udp server init success." : "udp server init failed.");
}

void UdpServer::stop(int32_t dwConnID)
{
    if (dwConnID < 0) mUdpServer->Stop();
    else mUdpServer->Disconnect(dwConnID);
}

void UdpServer::clear()
{
    // 关闭所有连接
    CONNID pIDs[65535] = { 0 };
    // dwCount 必须赋值，且要大于实际的连接数量才行，否则 GetAllConnectionIDs 返回 false
    DWORD dwCount = 65535;
    bool status = mUdpServer->GetAllConnectionIDs(pIDs, dwCount);
    if (status && (dwCount > 0))
    {
        for (uint64_t i = 0 ; i < dwCount; i++)
        {
            mUdpServer->Disconnect(pIDs[i]);
        }
    }
}

void UdpServer::close()
{
    mUdpServerListener.setActiveStatus(false);
    mUdpServer->Stop();
}

void UdpServer::send(const std::string &data, uint32_t length, int32_t dwConnID)
{
    mUdpServer->Send(dwConnID, (BYTE*)data.data(), length);
}
