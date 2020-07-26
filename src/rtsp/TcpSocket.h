#ifndef _TCPSOCKET_H_
#define _TCPSOCKET_H_
#include <string>
#include <stdint.h>

#include "Ipv4Address.h"

class  TcpSocket
{
public:
    explicit TcpSocket(int sockfd) :
        mSockfd(sockfd) { }

    ~TcpSocket();

    int getSocketFd() const { return mSockfd; }
    bool bind(Ipv4Address& addr);
    bool listen(int backlog);
    int accept();
    void setReuseAddr(int on);

private:
    int mSockfd;
};
#endif