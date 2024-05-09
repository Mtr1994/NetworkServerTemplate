#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "HPSocket/HPSocket.h"
#include "parser/messageparser.h"
#include "log/logger.h"

#include <string>
#include <stdint.h>
#include <iostream>

class TcpClientListener :  public CTcpClientListener
{
public:
    explicit TcpClientListener()
    {

    };

    inline void setActiveStatus(bool status) { mActiveStatus = status; };

    EnHandleResult OnPrepareConnect(ITcpClient* pSender, CONNID dwConnID, SOCKET socket) override
    {
        return HR_OK;
    }

    EnHandleResult OnConnect(ITcpClient* pSender, CONNID dwConnID) override
    {
        LOG_DEBUG("tcp socket connected.");
        return HR_OK;
    }

    EnHandleResult OnHandShake(ITcpClient* pSender, CONNID dwConnID) override
    {
        return HR_OK;
    }

    EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
    {
        if (!mActiveStatus) return HR_OK;
        std::string recvData = std::string((const char*)pData, iLength);
        MessageParser::getInstance()->parse(recvData);
        return HR_OK;
    }

    EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, int iLength) override
    {
        return HR_OK;
    }

    EnHandleResult OnSend(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
    {
        return HR_OK;
    }

    EnHandleResult OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override
    {
        LOG_DEBUG("tcp socket closed.");
        return HR_OK;
    }

private:
    bool mActiveStatus = true;
};

class TcpSocket 
{
public:
    explicit TcpSocket(const std::string& address, uint16_t port);
    void start();
    void stop(int32_t dwConnID);
    void clear();
    void close();
    void send(const std::string &data, uint32_t length, int32_t dwConnID);

private:
    std::string mPeerAddress;
    uint16_t mPeerPort;

    TcpClientListener mTcpClientListener;
    CTcpClientPtr mTcpClientPtr;
};


#endif // TCPSOCKET_H
