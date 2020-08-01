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
#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <string.h>
#include "Receiver.h"
#include "SocketOptions.h"
#include "Mylog.h"

#define msleep(x) usleep(x*1000)

Receiver* Receiver::createNew(const Ipv4Address& addr)
{
    return new Receiver(addr);
}

Receiver::Receiver(const Ipv4Address& addr) :
    mAddr(addr),
    mSocket(SocketOptions::createTcpSocket()),
    mNewConnectionCallback(NULL)
{
    //mSiganalThread = new SignalEvent();
    mSocket.setReuseAddr(1);
    mSocket.bind(mAddr);
}

Receiver::~Receiver()
{

}

void Receiver::listen()
{
    mListenning = true;
    mSocket.listen(1024);
    LOGI("listen ok\n");
}

void Receiver::setNewConnectionCallback(NewConnectionCallback cb, void* arg)
{
    mNewConnectionCallback = cb;
    mArg = arg;
}

void Receiver::readCallback(void* arg)
{
    Receiver* receiver = (Receiver*)arg;
    receiver->handleRead();
}

void Receiver::handleRead()
{
    // int connfd = mSocket.accept();
    // LOGI("client connect: %d\n", connfd);
    // if(mNewConnectionCallback)
    //     mNewConnectionCallback(mArg, connfd);
}
int Receiver::acceptClient(int sockfd, char* ip, int* port){
    int clientfd;
    socklen_t len = 0;
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));
    len = sizeof(addr);

    clientfd = accept(sockfd, (struct sockaddr *)&addr, &len);
    if(clientfd < 0){
        // LOGI("receive msg from client erro!\n");
        return -1;
    }
    strcpy(ip, inet_ntoa(addr.sin_addr));
    *port = ntohs(addr.sin_port);

    return clientfd;
}
void Receiver::acceptthread(void* accept){
    if(accept == NULL){
        LOGI("Receiver is NULL ,error\n");
        return;
    }
    Receiver* receiver = (Receiver*)accept;
    int serverSocketfd = receiver->getSocketfd();
    LOGI("Start do loop\n");
    while (1)
    {
        int fd;
        char clientIp[40];
        int port;
        fd = acceptClient(serverSocketfd, clientIp, &port);
        if(fd == -1){
            msleep(500);
            continue;
        }
        ClientInfo* clientinfo = new ClientInfo(fd,clientIp,port);
        receiver->doClientConnectCallback(clientinfo);
        LOGI("connect to client successful!\n");
        msleep(500);
    }
    
}
int Receiver::getSocketfd(){
    return mSocket.getSocketFd();
}
void Receiver::doClient(){
    std::thread accept(acceptthread,this);
    accept.detach();
    LOGI("create doClient thread ok\n");
}
void Receiver::doClientConnectCallback(ClientInfo* data){
    mNewConnectionCallback(mArg,data);
}

