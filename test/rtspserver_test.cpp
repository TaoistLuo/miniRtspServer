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