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
#ifndef _RTPINSTANNCE_H_
#define _RTPINSTANNCE_H_
#include <string>
#include <stdint.h>
#include <unistd.h>

#include "Ipv4Address.h"
#include "SocketOptions.h"
#include "Rtp.h"

class RtpInstance
{
public:
    enum RtpType
    {
        RTP_OVER_UDP,
        RTP_OVER_TCP
    };

    static RtpInstance* createNewOverUdp(int localSockfd, uint16_t localPort,
                                    std::string destIp, uint16_t destPort)
    {
        return new RtpInstance(localSockfd, localPort, destIp, destPort);
    }

    static RtpInstance* createNewOverTcp(int clientSockfd, uint8_t rtpChannel)
    {
        return new RtpInstance(clientSockfd, rtpChannel);
    }

    ~RtpInstance()
    { 
        SocketOptions::close(mServerSockfd);
    }

    uint16_t getLocalPort() const { return mServerPort; }
    uint16_t getPeerPort() { return mClientAddr.getPort(); }

    int send(RtpPacket* rtpPacket)
    {
        if(mRtpType == RTP_OVER_UDP)
        {
            return sendOverUdp(rtpPacket->mBuffer, rtpPacket->mSize);
        }
        else
        {
            uint8_t* rtpPktPtr = rtpPacket->_mBuffer;
            rtpPktPtr[0] = '$';
            rtpPktPtr[1] = (uint8_t)mRtpChannel;
            rtpPktPtr[2] = (uint8_t)(((rtpPacket->mSize)&0xFF00)>>8);
            rtpPktPtr[3] = (uint8_t)((rtpPacket->mSize)&0xFF);
            return sendOverTcp(rtpPktPtr, rtpPacket->mSize+4);
        }
    }

    bool alive() const { 
        return mIsAlive; 
        }
    int setAlive(bool alive) { mIsAlive = alive; };
    void setSessionId(uint16_t sessionId) { mSessionId = sessionId; }
    uint16_t getSessionId() const { return mSessionId; }

public:
    RtpInstance(int localSockfd, uint16_t localPort, const std::string& destIp, uint16_t destPort) :
        mRtpType(RTP_OVER_UDP), mServerSockfd(localSockfd), mServerPort(localPort),
        mClientAddr(destIp, destPort), mIsAlive(false), mSessionId(0)
    {
        
    }

    RtpInstance(int clientSockfd, uint8_t rtpChannel) :
        mRtpType(RTP_OVER_TCP), mServerSockfd(clientSockfd), 
        mIsAlive(false), mSessionId(0), mRtpChannel(rtpChannel)
    {
        
    }
private:
    int sendOverUdp(void* buf, int size)
    {
        return SocketOptions::sendto(mServerSockfd, buf, size, mClientAddr.getAddr());
    }

    int sendOverTcp(void* buf, int size)
    {
        return SocketOptions::write(mServerSockfd, buf, size);
    }


private:
    RtpType mRtpType;
    int mServerSockfd;
    uint16_t mServerPort; //for udp
    Ipv4Address mClientAddr; //for udp
    bool mIsAlive;
    uint16_t mSessionId;
    uint8_t mRtpChannel; //for tcp
};

#endif //_RTPINSTANNCE_H_