#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "RtspServer.h"
#include "H264FileMediaSource.h"
#include "H264RtpSink.h"
#include "Env.h"
using namespace std;

int main(){

    Ipv4Address addr(8554);
    Env* env = new Env();
    RtspServer* rtspserver = RtspServer::createNew(env,addr);
    MediaSession* session = MediaSession::createNew("lsl");
    MediaSource* videoSource = H264FileMediaSource::createNew(env,"enc.h264");
    
    RtpSink* rtph264Sink = H264RtpSink::createNew(env,videoSource);
    session->addRtpSink(MediaSession::TrackId0, rtph264Sink);
    rtspserver->addMeidaSession(session);
    rtspserver->start();
    
    while (1)
    {
        sleep(2);
    }
    

    return 0;
}