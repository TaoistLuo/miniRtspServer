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
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <algorithm>
#include <assert.h>

#include "MediaSession.h"
#include "SocketOptions.h"
#include "Mylog.h"
#include <unistd.h>
#include<iostream>
#include <cstring> 

MediaSession* MediaSession::createNew(std::string sessionName)
{
    return new MediaSession(sessionName);
}

MediaSession::MediaSession(const std::string& sessionName) :
    mSessionName(sessionName),
    mIsStartMulticast(false)
{
    mTracks[0].mTrackId = TrackId0;
    mTracks[1].mTrackId = TrackId1;
    mTracks[0].mIsAlive = false;
    mTracks[1].mIsAlive = false;

    for(int i = 0; i < MEDIA_MAX_TRACK_NUM; ++i)
    {
        mMulticastRtpInstances[i] = NULL;
        mMulticastRtcpInstances[i] = NULL;
    }
}

MediaSession::~MediaSession()
{
    for(int i = 0; i < MEDIA_MAX_TRACK_NUM; ++i)
    {
        if(mMulticastRtpInstances[i])
        {
            this->removeRtpInstance(mMulticastRtpInstances[i]);
            delete mMulticastRtpInstances[i];
        }

        if(mMulticastRtcpInstances[i]){
            delete mMulticastRtcpInstances[i];
        }
    }
}

std::string MediaSession::generateSDPDescription()
{
    if(!mSdp.empty())
        return mSdp;
    
    std::string ip = SocketOptions::getLocalIp();
    char buf[2048] = {0};

    snprintf(buf, sizeof(buf),
        "v=0\r\n"
        "o=- 9%ld 1 IN IP4 %s\r\n"
        "t=0 0\r\n"
        "a=control:*\r\n"
        "a=type:broadcast\r\n",
        (long)time(NULL), ip.c_str());

    if(isStartMulticast())
    {
        snprintf(buf+strlen(buf), sizeof(buf)-strlen(buf),
                "a=rtcp-unicast: reflection\r\n");
    }

    for(int i = 0; i < MEDIA_MAX_TRACK_NUM; ++i)
    {
        uint16_t port = 0;

        if(mTracks[i].mIsAlive != true)
            continue;

        if(isStartMulticast())
            port = getMulticastDestRtpPort((TrackId)i);
        
        snprintf(buf+strlen(buf), sizeof(buf)-strlen(buf),
                    "%s\r\n", mTracks[i].mRtpSink->getMediaDescription(port).c_str());

        if(isStartMulticast())
            snprintf(buf+strlen(buf), sizeof(buf)-strlen(buf),
                        "c=IN IP4 %s/255\r\n", getMulticastDestAddr().c_str());
        else
            snprintf(buf+strlen(buf), sizeof(buf)-strlen(buf),
                        "c=IN IP4 0.0.0.0\r\n");

        snprintf(buf+strlen(buf), sizeof(buf)-strlen(buf),
                    "%s\r\n", mTracks[i].mRtpSink->getAttribute().c_str());

        snprintf(buf+strlen(buf), sizeof(buf)-strlen(buf),											
                "a=control:track%d\r\n", mTracks[i].mTrackId);
    }

    mSdp = buf;
    LOGI("sdp:  %s",mSdp.c_str());
    return mSdp;
}

MediaSession::Track* MediaSession::getTrack(MediaSession::TrackId trackId)
{
    for(int i = 0; i < MEDIA_MAX_TRACK_NUM; ++i)
    {
        if(mTracks[i].mTrackId == trackId)
            return &mTracks[i];
    }

    return NULL;
}

bool MediaSession::addRtpSink(MediaSession::TrackId trackId, RtpSink* rtpSink)
{
    
    Track* track;

    track = getTrack(trackId);
    if(!track)
        return false;

    track->mRtpSink = rtpSink;
    track->mIsAlive = true;

    rtpSink->setSendFrameCallback(sendPacketCallback, this, track);

    return true;
}

bool MediaSession::addRtpInstance(MediaSession::TrackId trackId, RtpInstance* rtpInstance)
{
    Track* track = getTrack(trackId);
    if(!track || track->mIsAlive != true)
        return false;
    
    track->mRtpInstances.push_back(rtpInstance);

    return true;
}

bool MediaSession::removeRtpInstance(RtpInstance* rtpInstance)
{
    for(int i = 0; i <  MEDIA_MAX_TRACK_NUM; ++i)
    {
        if(mTracks[i].mIsAlive == false)
            continue;
        
        std::list<RtpInstance*>::iterator it = std::find(mTracks[i].mRtpInstances.begin(),
                                                            mTracks[i].mRtpInstances.end(),
                                                                rtpInstance);
        if(it == mTracks[i].mRtpInstances.end())
            continue;

        mTracks[i].mRtpInstances.erase(it);
        return true;
    }

    return false;
}

void MediaSession::sendPacketCallback(void* arg1, void* arg2, RtpPacket* rtpPacket)
{
    MediaSession* mediaSession = (MediaSession*)arg1;
    MediaSession::Track* track = (MediaSession::Track*)arg2;
    
    mediaSession->sendPacket(track, rtpPacket);
}

void MediaSession::sendPacket(MediaSession::Track* track, RtpPacket* rtpPacket)
{
    std::list<RtpInstance*>::iterator it;

    for(it = track->mRtpInstances.begin(); it != track->mRtpInstances.end(); ++it)
    {
        if((*it)->alive() == true)
        {
            (*it)->send(rtpPacket);
        }
    }
}

bool MediaSession::startMulticast()
{
    /* 随机生成多播地址 */
    struct sockaddr_in addr = { 0 };
    uint32_t range = 0xE8FFFFFF - 0xE8000100;
    addr.sin_addr.s_addr = htonl(0xE8000100 + (rand()) % range);
    mMulticastAddr = inet_ntoa(addr.sin_addr);

    int rtpSockfd1, rtcpSockfd1;
    int rtpSockfd2, rtcpSockfd2;
    uint16_t rtpPort1, rtcpPort1;
    uint16_t rtpPort2, rtcpPort2;
    bool ret;

    rtpSockfd1 = SocketOptions::createUdpSocket();
    assert(rtpSockfd1 > 0);

    rtpSockfd2 = SocketOptions::createUdpSocket();
    assert(rtpSockfd2 > 0);

    rtcpSockfd1 = SocketOptions::createUdpSocket();
    assert(rtcpSockfd1 > 0);

    rtcpSockfd2 = SocketOptions::createUdpSocket();
    assert(rtcpSockfd2 > 0);

    uint16_t port = rand() & 0xfffe;
    if(port < 10000){
        port += 10000;
    }
    
    rtpPort1 = port;
    rtcpPort1 = port+1;
    rtpPort2 = rtcpPort1+1;
    rtcpPort2 = rtpPort2+1;

    mMulticastRtpInstances[TrackId0] = RtpInstance::createNewOverUdp(rtpSockfd1, 0, mMulticastAddr, rtpPort1);
    mMulticastRtpInstances[TrackId1] = RtpInstance::createNewOverUdp(rtpSockfd2, 0, mMulticastAddr, rtpPort2);    
    mMulticastRtcpInstances[TrackId0] = RtcpInstance::createNew(rtcpSockfd1, 0, mMulticastAddr, rtcpPort1);
    mMulticastRtcpInstances[TrackId1] = RtcpInstance::createNew(rtcpSockfd2, 0, mMulticastAddr, rtcpPort2);    

    this->addRtpInstance(TrackId0, mMulticastRtpInstances[TrackId0]);
    this->addRtpInstance(TrackId1, mMulticastRtpInstances[TrackId1]);
    mMulticastRtpInstances[TrackId0]->setAlive(true);
    mMulticastRtpInstances[TrackId1]->setAlive(true);

    mIsStartMulticast = true;

    return true;
}

bool MediaSession::isStartMulticast()
{
    return mIsStartMulticast;
}

uint16_t MediaSession::getMulticastDestRtpPort(TrackId trackId)
{
    if(trackId > TrackId1 || !mMulticastRtpInstances[trackId])
        return -1;

    return mMulticastRtpInstances[trackId]->getPeerPort();
}