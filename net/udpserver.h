#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "HPSocket/HPSocket.h"
#include "parser/messageparser.h"

#include <string>
#include <stdint.h>
#include <iostream>

class UdpServerListener : public CUdpServerListener
{
public:
    explicit UdpServerListener() { }

    inline void setActiveStatus(bool status) { mActiveStatus = status; };

    EnHandleResult OnPrepareListen(IUdpServer* pSender, SOCKET soListen) override
    {
        return HR_OK;
    }

    // 函数会在连接第一次发送数据的时候触发
    EnHandleResult OnAccept(IUdpServer* pSender, CONNID dwConnID, UINT_PTR pSockAddr) override
    {
        return HR_OK;
    }

    EnHandleResult OnHandShake(IUdpServer* pSender, CONNID dwConnID) override
    {
        return HR_OK;
    }

    EnHandleResult OnSend(IUdpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
    {
        return HR_OK;
    }

    EnHandleResult OnReceive(IUdpServer* pSender, CONNID dwConnID, int iLength) override
    {
        return HR_OK;
    }

    EnHandleResult OnReceive(IUdpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
    {
        std::string recvData = std::string((char*)pData, iLength);
        MessageParser::getInstance()->parse(recvData);
        return HR_OK;
    }

    EnHandleResult OnClose(IUdpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override
    {
        return HR_OK;
    }

    EnHandleResult OnShutdown(IUdpServer* pSender) override
    {
        return HR_OK;
    }

private:
    bool mActiveStatus = true;
};

class UdpServer
{
public:
    explicit UdpServer(const std::string& address, uint16_t port);
    void start() ;
    void stop(int32_t dwConnID) ;
    void clear() ;
    void close();
    void send(const std::string &data, uint32_t length, int32_t dwConnID) ;

private:
    std::string mLocalAddress;
    uint16_t mLocalPort;

    CUdpServerPtr mUdpServer;
    UdpServerListener mUdpServerListener;
};

#endif // UDPSERVER_H
