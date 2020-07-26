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