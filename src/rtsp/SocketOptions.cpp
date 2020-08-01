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
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <string.h>

#include "SocketOptions.h"
#include "Mylog.h"

int SocketOptions::createTcpSocket()
{
  int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if(sockfd < 0){
        return -1;
    }
  return sockfd;
}

int SocketOptions::createUdpSocket()
{
  int sockfd = ::socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);

  return sockfd;
}

bool SocketOptions::bind(int sockfd, std::string ip, uint16_t port)
{
    struct sockaddr_in addr = {0};			  
    addr.sin_family = AF_INET;		  
    //addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    addr.sin_port = htons(port);

    if(::bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        LOGI("bind Failed\n");
        return false;
    }

    return true;
}

bool SocketOptions::listen(int sockfd, int backlog)
{
    if(::listen(sockfd, backlog) < 0){
        LOGI("failed to listen\n");
        return false;
    }
    return true;
}

int SocketOptions::accept(int sockfd)
{
    struct sockaddr_in addr = { 0 };
    socklen_t addrlen = sizeof(struct sockaddr_in);

    int connfd = ::accept(sockfd, (struct sockaddr*)&addr, &addrlen);
    setNonBlockAndCloseOnExec(connfd);
    ignoreSigPipeOnSocket(connfd);

    return connfd;
}

int SocketOptions::readv(int sockfd, const struct iovec *iov, int iovcnt)
{
    return ::readv(sockfd, iov, iovcnt);
}

int SocketOptions::write(int sockfd, const void* buf, int size)
{
    return ::write(sockfd, buf, size);
}

int SocketOptions::sendto(int sockfd, const void* buf, int len, 
                        const struct sockaddr *destAddr)
{
    socklen_t addrLen = sizeof(struct sockaddr);
    return ::sendto(sockfd, buf, len, 0, destAddr, addrLen);
}

void SocketOptions::setNonBlock(int sockfd)
{
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

void SocketOptions::setBlock(int sockfd, int writeTimeout)
{
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags&(~O_NONBLOCK));

    if (writeTimeout > 0)
    {
        struct timeval tv = {writeTimeout/1000, (writeTimeout%1000)*1000};
        setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof(tv));
    }
}

void SocketOptions::setReuseAddr(int sockfd, int on)
{
    int optval = on ? 1 : 0;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval));
}

void SocketOptions::setReusePort(int sockfd)
{
#ifdef SO_REUSEPORT
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (const char*)&on, sizeof(on));
#endif
}

void SocketOptions::setNonBlockAndCloseOnExec(int sockfd)
{
  // non block,asynchronous
  int flags = ::fcntl(sockfd, F_GETFL, 0);
  flags |= O_NONBLOCK;
  int ret = ::fcntl(sockfd, F_SETFL, flags);

  // close on exec
  flags = ::fcntl(sockfd, F_GETFD, 0);
  flags |= FD_CLOEXEC;
  ret = ::fcntl(sockfd, F_SETFD, flags);
}

void SocketOptions::ignoreSigPipeOnSocket(int socketfd)
{
    int option = 1;
    setsockopt(socketfd, SOL_SOCKET, MSG_NOSIGNAL, &option, sizeof(option));
}

void SocketOptions::setNoDelay(int sockfd)
{
#ifdef TCP_NODELAY
    int on = 1;
    int ret = setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *)&on, sizeof(on));
#endif
}

void SocketOptions::setKeepAlive(int sockfd)
{
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof(on));
}

void SocketOptions::setNoSigpipe(int sockfd)
{
#ifdef SO_NOSIGPIPE
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_NOSIGPIPE, (char *)&on, sizeof(on));
#endif
}

void SocketOptions::setSendBufSize(int sockfd, int size)
{
    setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (char *)&size, sizeof(size));
}

void SocketOptions::setRecvBufSize(int sockfd, int size)
{
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char *)&size, sizeof(size));
}

std::string SocketOptions::getPeerIp(int sockfd)
{
    struct sockaddr_in addr = { 0 };
    socklen_t addrlen = sizeof(struct sockaddr_in);
    if (getpeername(sockfd, (struct sockaddr *)&addr, &addrlen) == 0)
    {
        return inet_ntoa(addr.sin_addr);
    }

    return "0.0.0.0";
}

int16_t SocketOptions::getPeerPort(int sockfd)
{
    struct sockaddr_in addr = { 0 };
    socklen_t addrlen = sizeof(struct sockaddr_in);
    if (getpeername(sockfd, (struct sockaddr *)&addr, &addrlen) == 0)
    {
        return ntohs(addr.sin_port);
    }

    return 0;
}

int SocketOptions::getPeerAddr(int sockfd, struct sockaddr_in *addr)
{
    socklen_t addrlen = sizeof(struct sockaddr_in);
    return getpeername(sockfd, (struct sockaddr *)addr, &addrlen);
}

void SocketOptions::close(int sockfd)
{
    int ret = ::close(sockfd);
}

bool SocketOptions::connect(int sockfd, std::string ip, uint16_t port, int timeout)
{
	bool isConnected = true;
	if (timeout > 0)
	{
		SocketOptions::setNonBlock(sockfd);
	}

	struct sockaddr_in addr = { 0 };
	socklen_t addrlen = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	if (::connect(sockfd, (struct sockaddr*)&addr, addrlen) < 0)
	{
		if (timeout > 0)
		{
			isConnected = false;
			fd_set fdWrite;
			FD_ZERO(&fdWrite);
			FD_SET(sockfd, &fdWrite);
			struct timeval tv = { timeout / 1000, timeout % 1000 * 1000 };
			select(sockfd + 1, NULL, &fdWrite, NULL, &tv);
			if (FD_ISSET(sockfd, &fdWrite))
			{
				isConnected = true;
			}
			SocketOptions::setBlock(sockfd, 0);
		}
		else
		{
			isConnected = false;
		}		
	}
	
	return isConnected;
}

std::string SocketOptions::getLocalIp()
{
    int sockfd = 0;
    char buf[512] = { 0 };
    struct ifconf ifconf;
    struct ifreq  *ifreq;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        close(sockfd);
        return "0.0.0.0";
    }

    ifconf.ifc_len = 512;
    ifconf.ifc_buf = buf;
    if (ioctl(sockfd, SIOCGIFCONF, &ifconf) < 0)
    {
        close(sockfd);
        return "0.0.0.0";
    }

    close(sockfd);

    ifreq = (struct ifreq*)ifconf.ifc_buf;
    for (int i = (ifconf.ifc_len / sizeof(struct ifreq)); i>0; i--)
    {
        if (ifreq->ifr_flags == AF_INET)
        {
            if (strcmp(ifreq->ifr_name, "lo") != 0)
            {
                return inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr);
            }
            ifreq++;
        }
    }
    return "0.0.0.0";
}