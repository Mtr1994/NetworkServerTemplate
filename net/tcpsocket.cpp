#include "tcpsocket.h"
#include "log/logger.h"

#include <iostream>

using namespace  std;

TcpSocket::TcpSocket(const std::string &address, uint16_t port)
    : mPeerAddress(address), mPeerPort(port)
{
    mTcpClientPtr.Attach(TcpClient_Creator::Create(&mTcpClientListener));
}

void TcpSocket::start()
{
    // 初始化消息解析线程
    MessageParser::getInstance()->init();
    
    if (mTcpClientPtr->GetState() <= SS_STARTED) return;
    bool status = mTcpClientPtr->Start(LPCTSTR(mPeerAddress.data()), mPeerPort);
    LOG_DEBUG(status ? "tcp socket start success." : "tcp socket start failed.");
}

void TcpSocket::stop(int32_t dwConnID)
{
    mTcpClientPtr->Stop();
}

void TcpSocket::clear()
{
    // nothing to do here
}

void TcpSocket::close()
{
    mTcpClientListener.setActiveStatus(false);
    mTcpClientPtr->Stop();
}

void TcpSocket::send(const std::string &data, uint32_t length, int32_t dwConnID)
{
    if (!mTcpClientPtr.IsValid()) return;
    mTcpClientPtr->Send((BYTE*)data.data(), (int)length);
}

