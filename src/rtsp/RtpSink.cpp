#include <arpa/inet.h>
#include "RtpSink.h"
#include "Mylog.h"
RtpSink::RtpSink(Env* env,MediaSource* mediaSource, int payloadType) :
    mMediaSource(mediaSource),
    mSendPacketCallback(NULL),
    mCsrcLen(0),
    mExtension(0),
    mPadding(0),
    mVersion(RTP_VESION),
    mPayloadType(payloadType),
    mMarker(0),
    mSeq(0),
    mTimestamp(0),
    mEnv(env)
    
{
    uint32_t frameTime = 1000/(mMediaSource->getFps());
    mEnv->addTimer(timeoutCallback,frameTime,true,this);
    mSSRC = rand();
}

RtpSink::~RtpSink()
{
    delete mTimerEvent;
}

void RtpSink::setSendFrameCallback(SendPacketCallback cb, void* arg1, void* arg2)
{
    mSendPacketCallback = cb;
    mArg1 = arg1;
    mArg2 = arg2;
}

void RtpSink::sendRtpPacket(RtpPacket* packet)
{
    RtpHeader* rtpHead = packet->mRtpHeadr;
    rtpHead->csrcLen = mCsrcLen;
    rtpHead->extension = mExtension;
    rtpHead->padding = mPadding;
    rtpHead->version = mVersion;
    rtpHead->payloadType = mPayloadType;
    rtpHead->marker = mMarker;
    rtpHead->seq = htons(mSeq);
    rtpHead->timestamp = htonl(mTimestamp);
    rtpHead->ssrc = htonl(mSSRC);
    packet->mSize += RTP_HEADER_SIZE;
    
    if(mSendPacketCallback)
        mSendPacketCallback(mArg1, mArg2, packet);
}

void RtpSink::timeoutCallback(void* arg)
{
    RtpSink* rtpSink = (RtpSink*)arg;
    AVFrame* frame = rtpSink->mMediaSource->getFrame();
    if(!frame)
    {
        LOGI("get frame was NULL\n");
        return;
    }

    rtpSink->handleFrame(frame);

    rtpSink->mMediaSource->putFrame(frame);
    
}

void RtpSink::start(int ms)
{
    //mTimerEvent->setTimeout((uint32_t)ms);
    mTimerEvent->start(ms*1000,true);
}

void RtpSink::stop()
{
    //mEnv->scheduler()->removeTimedEvent(mTimerId);
}