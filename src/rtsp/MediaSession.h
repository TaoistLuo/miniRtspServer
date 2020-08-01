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
#ifndef _MEDIASESSION_H_
#define _MEDIASESSION_H_
#include <string>
#include <list>

#include "RtpInstance.h"
#include "RtcpInstance.h"
#include "RtpSink.h"

#define MEDIA_MAX_TRACK_NUM 2

class MediaSession
{
public:
    enum TrackId
    {
        TrackIdNone = -1,
        TrackId0    = 0,
        TrackId1    = 1,
    };

    static MediaSession* createNew(std::string sessionName);

    MediaSession(const std::string& sessionName);
    ~MediaSession();

    std::string name() const { return mSessionName; }
    std::string generateSDPDescription();
    bool addRtpSink(MediaSession::TrackId trackId, RtpSink* rtpSink);
    bool addRtpInstance(MediaSession::TrackId trackId, RtpInstance* rtpInstance);
    bool removeRtpInstance(RtpInstance* rtpInstance);
    bool startMulticast();
    bool isStartMulticast();
    std::string getMulticastDestAddr() const { return mMulticastAddr; }
    uint16_t getMulticastDestRtpPort(TrackId trackId);

public:
    class Track
    {
    public:
        RtpSink* mRtpSink;
        int mTrackId;
        bool mIsAlive;
        std::list<RtpInstance*> mRtpInstances;
    };

    Track* getTrack(MediaSession::TrackId trackId);
    static void sendPacketCallback(void* arg1, void* arg2, RtpPacket* rtpPacket);
    void sendPacket(MediaSession::Track* tarck, RtpPacket* rtpPacket);

private:
    std::string mSessionName;
    std::string mSdp;
    Track mTracks[MEDIA_MAX_TRACK_NUM];
    bool mIsStartMulticast;
    std::string mMulticastAddr;
    RtpInstance* mMulticastRtpInstances[MEDIA_MAX_TRACK_NUM];
    RtcpInstance* mMulticastRtcpInstances[MEDIA_MAX_TRACK_NUM];
};

#endif //_MEDIASESSION_H_