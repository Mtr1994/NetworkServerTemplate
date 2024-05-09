#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "HPSocket/HPSocket.h"
#include "parser/messageparser.h"

#include <string>
#include <stdint.h>
#include <iostream>

class TcpSocketListener :  public CTcpServerListener
{
public:
    explicit TcpSocketListener()
    {

    }

    inline void setActiveStatus(bool status) { mActiveStatus = status; };

    EnHandleResult OnPrepareListen(ITcpServer* pSender, SOCKET soListen) override
    {
        return HR_OK;
    }

    EnHandleResult OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient) override
    {
        return HR_OK;
    }

    EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
    {
        std::string recvData = std::string((const char*)pData, iLength);
        MessageParser::getInstance()->parse(recvData);
        return HR_OK;
    }

    EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, int iLength) override
    {
        return HR_OK;
    }

    EnHandleResult OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override
    {
        return HR_OK;
    }

    EnHandleResult OnShutdown(ITcpServer* pSender) override
    {
        return HR_OK;
    }

private:
    bool mActiveStatus = true;
};

class TcpServer
{
public:
    explicit TcpServer(const std::string& address, uint16_t port);
    void start();
    void stop(int32_t dwConnID);
    void clear();
    void close();
    void send(const std::string &data, uint32_t length, int32_t dwConnID);

private:
    std::string mLocalAddress;
    uint16_t mLocalPort;

    CTcpServerPtr mTcpPackServer;
    TcpSocketListener mTcpSocketListener;
};

#endif // TCPSERVER_H
