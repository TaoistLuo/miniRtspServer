/*************************************************************************
Copyright (c) 2020 Taoist Luo

Create by: Taoist Luo
CSDN：https://editor.csdn.net/md/?articleId=107549026

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*****************************************************************************/
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