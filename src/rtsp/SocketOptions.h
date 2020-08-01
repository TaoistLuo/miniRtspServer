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
#ifndef _SOCKETOPTION_H_
#define _SOCKETOPTION_H_
#include <string>
#include <arpa/inet.h>
#include <sys/uio.h>

namespace SocketOptions
{
int createTcpSocket();
int createUdpSocket();
bool bind(int sockfd, std::string ip, uint16_t port);
bool listen(int sockfd, int backlog);
int accept(int sockfd);
int readv(int sockfd, const struct iovec *iov, int iovcnt);
int write(int sockfd, const void* buf, int size);
int sendto(int sockfd, const void* buf, int len, const struct sockaddr *destAddr);
void setNonBlock(int sockfd);
void setBlock(int sockfd, int writeTimeout);
void setReuseAddr(int sockfd, int on);
void setReusePort(int sockfd);
void setNonBlockAndCloseOnExec(int sockfd);
void ignoreSigPipeOnSocket(int socketfd);
void setNoDelay(int sockfd);
void setKeepAlive(int sockfd);
void setNoSigpipe(int sockfd);
void setSendBufSize(int sockfd, int size);
void setRecvBufSize(int sockfd, int size);
std::string getPeerIp(int sockfd);
int16_t getPeerPort(int sockfd);
int getPeerAddr(int sockfd, struct sockaddr_in *addr);
void close(int sockfd);
bool connect(int sockfd, std::string ip, uint16_t port, int timeout);
std::string getLocalIp();
}

#endif 