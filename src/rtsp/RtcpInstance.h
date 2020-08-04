/*************************************************************************
Copyright (c) 2020 Taoist Luo

Create by: Taoist Luo
CSDN：https://blog.csdn.net/daichanzhang9734/article/details/107549026

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
#ifndef _RTCP_H_
#define _RTCP_H_
#include <string>
#include <stdint.h>
#include <unistd.h>

#include "Ipv4Address.h"
#include "SocketOptions.h"
class RtcpInstance
{
public:
    static RtcpInstance* createNew(int localSockfd, uint16_t localPort,
                                    std::string destIp, uint16_t destPort)
    {
        return new RtcpInstance(localSockfd, localPort, destIp, destPort);
    }

    ~RtcpInstance()
    {
        SocketOptions::close(mServerSockfd);
    }

    int send(void* buf, int size)
    {
        return SocketOptions::sendto(mServerSockfd, buf, size, mDestAddr.getAddr());
    }

    int recv(void* buf, int size, Ipv4Address* addr)
    {
        return 0;
    }

    uint16_t getLocalPort() const { return mLocalPort; }

    int alive() const { return mIsAlive; }
    int setAlive(bool alive) { mIsAlive = alive; };
    void setSessionId(uint16_t sessionId) { mSessionId = sessionId; }
    uint16_t sessionId() const { return mSessionId; }

public:
    RtcpInstance(int localSockfd, uint16_t localPort,
                    std::string destIp, uint16_t destPort) :
        mServerSockfd(localSockfd), mLocalPort(localPort), mDestAddr(destIp, destPort),
        mIsAlive(false), mSessionId(0)
    {   }

private:
    int mServerSockfd;
    uint16_t mLocalPort;
    Ipv4Address mDestAddr;
    bool mIsAlive;
    uint16_t mSessionId;
};


#endif