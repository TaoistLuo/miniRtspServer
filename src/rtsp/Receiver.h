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