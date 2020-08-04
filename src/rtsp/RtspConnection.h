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
#ifndef _RTSP_CONNECTION__
#define _RTSP_CONNECTION__
#include <map>

#include "TcpConnect.h"
#include "RtspServer.h"
#include "RtpInstance.h"
#include "MediaSession.h"
#include "Event.h"

class RtspServer;
class RtspConnection : public TcpConnect
{
public:
    /*
    *The Rtspserver command
    */
    enum Method
    {
        OPTIONS, DESCRIBE, SETUP, PLAY, TEARDOWN, GET_PARAMETER, RTCP,
        NONE,
    };
    static void getSocketIp(int sockfd, std::string& ip);
    static RtspConnection* createNew(RtspServer* rtspServer, int sockfd);
    
    RtspConnection(RtspServer* rtspServer, int sockfd);
    ~RtspConnection();

protected:
    virtual void handleReadBytes();

private:
    bool parseRequest();
    bool parseRequest1(const char* begin, const char* end);
    bool parseRequest2(const char* begin, const char* end);

    bool parseCSeq(std::string& message);
    bool parseAccept(std::string& message);
    bool parseTransport(std::string& message);
    bool parseMediaTrack();
    bool parseSessionId(std::string& message);

    bool handleCmdOption();
    bool handleCmdDescribe();
    bool handleCmdSetup();
    bool handleCmdPlay();
    bool handleCmdTeardown();
    bool handleCmdGetParamter();

    int sendMessage(void* buf, int size);
    int sendMessage();

    bool createRtpRtcpOverUdp(MediaSession::TrackId trackId, std::string peerIp,
                        uint16_t peerRtpPort, uint16_t peerRtcpPort);
    bool createRtpOverTcp(MediaSession::TrackId trackId, int sockfd, uint8_t rtpChannel);

    void handleRtpOverTcp();

private:
    RtspServer* mRtspServer;
    std::string mPeerIp;
    Method mMethod;
    std::string mUrl;
    std::string mSuffix;
    uint32_t mCSeq;
    uint16_t mPeerRtpPort;
    uint16_t mPeerRtcpPort;
    MediaSession::TrackId mTrackId;
    RtpInstance* mRtpInstances[MEDIA_MAX_TRACK_NUM];
    RtcpInstance* mRtcpInstances[MEDIA_MAX_TRACK_NUM];
    MediaSession* mSession;
    int mSessionId;
    bool mIsRtpOverTcp;
    uint8_t mRtpChannel;
};


#endif