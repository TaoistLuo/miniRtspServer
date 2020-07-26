#include "TcpConnect.h"
#include "SocketOptions.h"
#include "Mylog.h"
#include <unistd.h>
#include <stdlib.h>

TcpConnect::TcpConnect(int sockfd) :
    mSocket(sockfd),
    mDisconnectionCallback(NULL),
    mArg(NULL)
{
    LOGI("Start constructor TcpConnect\n");
    mTcpConnIOEvent = IOEvent::createNew(sockfd, this);
    mTcpConnIOEvent->setReadCallback(readCallback);
    mTcpConnIOEvent->setWriteCallback(writeCallback);
    mTcpConnIOEvent->setErrorCallback(errorCallback);
    mTcpConnIOEvent->enableReadHandling(); //默认只开启读
    mTcpConnIOEvent->setREvent(1); //默认只开启读
    //创建线程
    mTcpConnIOEvent->start();
    LOGI("Start mTcpConnIOEvent successful\n");
}

TcpConnect::~TcpConnect()
{

}

void TcpConnect::setDisconnectionCallback(DisconnectionCallback callback, void* arg)
{
    mDisconnectionCallback = callback;
    mArg = arg;
}

void TcpConnect::enableReadHandling()
{
    if(mTcpConnIOEvent->isReadHandling()){
        return;
    }
    mTcpConnIOEvent->enableReadHandling();
}

void TcpConnect::enableWriteHandling()
{
    if(mTcpConnIOEvent->isWriteHandling())
        return;
    
    mTcpConnIOEvent->enableWriteHandling();
}

void TcpConnect::enableErrorHandling()
{
    if(mTcpConnIOEvent->isErrorHandling())
        return;

    mTcpConnIOEvent->enableErrorHandling();
}

void TcpConnect::disableReadeHandling()
{
    if(!mTcpConnIOEvent->isReadHandling())
        return;

    mTcpConnIOEvent->disableReadeHandling();
}   

void TcpConnect::disableWriteHandling()
{
    if(!mTcpConnIOEvent->isWriteHandling())
        return;

    mTcpConnIOEvent->disableWriteHandling();
}

void TcpConnect::disableErrorHandling()
{
    if(!mTcpConnIOEvent->isErrorHandling())
        return;

    mTcpConnIOEvent->disableErrorHandling();
}

void TcpConnect::handleRead()
{
    int ret = mInputBuffer.read(mSocket.getSocketFd());

    if(ret == 0)
    {
        LOGI("client disconnect\n");
        handleDisconnection();
        return;
    }else if(ret < 0)
    {
        LOGI("read err\n");
        handleDisconnection();
        return;
    }
    handleReadBytes();
}

void TcpConnect::handleReadBytes()
{
    LOGI("default read handle\n");
    mInputBuffer.retrieveAll();
}

void TcpConnect::handleWrite()
{
    LOGI("default wirte handle\n");
    mOutBuffer.retrieveAll();
}

void TcpConnect::handleError()
{
    LOGI("default error handle\n");
}

void TcpConnect::readCallback(void* arg)
{
    TcpConnect* Connect = (TcpConnect*)arg;
    Connect->handleRead();
}

void TcpConnect::writeCallback(void* arg)
{
    TcpConnect* Connect = (TcpConnect*)arg;
    Connect->handleWrite();
}

void TcpConnect::errorCallback(void* arg)
{
    TcpConnect* Connect = (TcpConnect*)arg;
    Connect->handleError();
}

void TcpConnect::handleDisconnection()
{
    if(mDisconnectionCallback)
        mDisconnectionCallback(mArg, mSocket.getSocketFd());
}