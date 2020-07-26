#ifndef _ACCEPTOR_H__
#define _ACCEPTOR_H__
#include <string.h>
#include "Ipv4Address.h"
#include "SocketOptions.h"
#include "TcpSocket.h"
#include "SignalEvent.h"

struct ClientInfo{
    int clientSockfd;
    char clientIp[40];
    int clientPort;
    ClientInfo(int fd, char* Ip,int port){
        clientSockfd = fd;
        memset(clientIp,0,40);
        strcpy(clientIp,Ip);
        clientPort = port;
    }
};

class Receiver
{
public:
    typedef void(*NewConnectionCallback)(void *tcpserver, ClientInfo* data);

    static Receiver* createNew(const Ipv4Address& addr);

    Receiver(const Ipv4Address& addr);
    ~Receiver();

    bool listenning() const { return mListenning; }
    void listen();
    void setNewConnectionCallback(NewConnectionCallback cb, void* arg);
    static void acceptthread(void* accept);
    static int acceptClient(int sockfd, char* ip, int* port);
    int getSocketfd();
    void doClient();
    void doClientConnectCallback(ClientInfo* data);
private:
    static void readCallback(void*);
    void handleRead();

private:
    Ipv4Address mAddr;
    TcpSocket mSocket;
    bool mListenning;
    NewConnectionCallback mNewConnectionCallback;
    SignalEvent* mSiganalThread;
    void* mArg;
};
#endif