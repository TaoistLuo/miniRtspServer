#ifndef _TCP_CONNECT_H__
#define _TCP_CONNECT_H__
#include "TcpSocket.h"
#include "DataBuffer.h"
#include "Event.h"

class TcpConnect
{
public:
    typedef void (*DisconnectionCallback)(void*, int);

    TcpConnect(int sockfd);
    virtual ~TcpConnect();

    void setDisconnectionCallback(DisconnectionCallback callback, void* arg);

protected:
    void enableReadHandling();
    void enableWriteHandling();
    void enableErrorHandling();
    void disableReadeHandling();
    void disableWriteHandling();
    void disableErrorHandling();

    void handleRead();
    virtual void handleReadBytes();
    virtual void handleWrite();
    virtual void handleError();

    void handleDisconnection();

private:
    static void readCallback(void* arg);
    static void writeCallback(void* arg);
    static void errorCallback(void* arg);

protected:
    TcpSocket mSocket;
    IOEvent* mTcpConnIOEvent;
    DisconnectionCallback mDisconnectionCallback;
    void* mArg;
    DataBuffer mInputBuffer;
    DataBuffer mOutBuffer;
    char mBuffer[2048];
};

#endif