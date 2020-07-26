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

