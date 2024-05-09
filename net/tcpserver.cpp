#include "tcpserver.h"
#include "log/logger.h"

#include <iostream>

using namespace  std;

TcpServer::TcpServer(const std::string &address, uint16_t port)
    : mLocalAddress(address), mLocalPort(port)
{
    mTcpPackServer.Attach(TcpServer_Creator::Create(&mTcpSocketListener));
}

void TcpServer::start()
{
    // 初始化消息解析线程
    MessageParser::getInstance()->init();

    if (mTcpPackServer->GetState() <= SS_STARTED) return;
    bool status = mTcpPackServer->Start(LPCTSTR(mLocalAddress.data()), mLocalPort);
    LOG_DEBUG(status ? "tcp server init success." : "tcp server init failed.");
}

void TcpServer::stop(int32_t dwConnID)
{
    if (dwConnID < 0) mTcpPackServer->Stop();
    else mTcpPackServer->Disconnect(dwConnID);
}

void TcpServer::clear()
{
    // 关闭所有连接
    CONNID pIDs[65535] = { 0 };
    // dwCount 必须赋值，且要大于实际的连接数量才行，否则 GetAllConnectionIDs 返回 false
    DWORD dwCount = 65535;
    bool status = mTcpPackServer->GetAllConnectionIDs(pIDs, dwCount);
    if (status && (dwCount > 0))
    {
        for (uint64_t i = 0 ; i < dwCount; i++)
        {
            mTcpPackServer->Disconnect(pIDs[i]);
        }
    }
}

void TcpServer::close()
{
    mTcpSocketListener.setActiveStatus(false);
    mTcpPackServer->Stop();
}

void TcpServer::send(const std::string &data, uint32_t length, int32_t dwConnID)
{
    mTcpPackServer->Send(dwConnID, (BYTE*)data.data(), length);
}
