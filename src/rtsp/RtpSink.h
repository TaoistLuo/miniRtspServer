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
#ifndef _MEDIA_SINK_H_
#define _MEDIA_SINK_H_
#include <string>
#include <stdint.h>

#include "MediaSource.h"
#include "Rtp.h"
#include "Event.h"
#include "TimeEventManager.h"
#include "Env.h"

class RtpSink
{
public:
    typedef void (*SendPacketCallback)(void* arg1, void* arg2, RtpPacket* mediaPacket);

    RtpSink(Env* env,MediaSource* mediaSource, int payloadType);
    virtual ~RtpSink();

    virtual std::string getMediaDescription(uint16_t port) = 0;
    virtual std::string getAttribute() = 0;

    void setSendFrameCallback(SendPacketCallback cb, void* arg1, void* arg2);

protected:
    virtual void handleFrame(AVFrame* frame) = 0;
    void sendRtpPacket(RtpPacket* packet);
    void start(int ms);
    void stop();

private:
    static void timeoutCallback(void*);

protected:
    MediaSource* mMediaSource;
    SendPacketCallback mSendPacketCallback;
    void* mArg1;
    void* mArg2;

    uint8_t mCsrcLen;
    uint8_t mExtension;
    uint8_t mPadding;
    uint8_t mVersion;
    uint8_t mPayloadType;
    uint8_t mMarker;
    uint16_t mSeq;
    uint32_t mTimestamp;
    uint32_t mSSRC;

private:
     //TimerEvent* mTimerEvent;
     TimeEventManager* mTimerEvent;
     //Timer::TimerId mTimerId;
     Env* mEnv;
};

#endif //_MEDIA_SINK_H_