#include <unistd.h>

#include "TcpSocket.h"
#include "SocketOptions.h"

TcpSocket::~TcpSocket()
{
    SocketOptions::close(mSockfd);
}

bool TcpSocket::bind(Ipv4Address& addr)
{
    return SocketOptions::bind(mSockfd, addr.getIp(), addr.getPort());
}

bool TcpSocket::listen(int backlog)
{
    return SocketOptions::listen(mSockfd, backlog);
}

int TcpSocket::accept()
{
    return SocketOptions::accept(mSockfd);
}

void TcpSocket::setReuseAddr(int on)
{
    SocketOptions::setReuseAddr(mSockfd, on);
}