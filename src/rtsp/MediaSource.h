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
#ifndef _MEDIA_SOURCE_H_
#define _MEDIA_SOURCE_H_
#include <queue>
#include <stdint.h>
#include <unistd.h>
#include<iostream>
#include <cstring> 
#include "Mutex.h"
#include "Event.h"
#include "TimeEventManager.h"
#include "SignalEvent.h"
#include "Env.h"

#define FRAME_MAX_SIZE (1024*500)
#define DEFAULT_FRAME_NUM 4
class AVFrame1
{
public:
    AVFrame1(int framesize,char* frame) { 
        mFrame = new char[framesize],
        memcpy(mFrame,frame,framesize);
        mFrameSize = framesize;
    }

    ~AVFrame1()
    { delete mBuffer; }

    char* mBuffer;
    char* mFrame;
    int mFrameSize;
};
class AVFrame
{
public:
    AVFrame() :
        mBuffer(new uint8_t[FRAME_MAX_SIZE]),
        mFrameSize(0)
    { }

    ~AVFrame()
    { delete mBuffer; }

    uint8_t* mBuffer;
    uint8_t* mFrame;
    int mFrameSize;
};

class MediaSource
{
public:
    virtual ~MediaSource();

    AVFrame* getFrame();
    void putFrame(AVFrame* frame);
    int getFps() const { return mFps; }
    void start(uint32_t ms);

protected:
    MediaSource(Env* env);
    virtual void readFrame() = 0;
    void setFps(int fps) { mFps = fps; }

private:
    static void taskCallback(void*);

protected:
    Env* mEnv;
    AVFrame mAVFrames[DEFAULT_FRAME_NUM];
    std::queue<AVFrame*> mAVFrameInputQueue;
    std::queue<AVFrame*> mAVFrameOutputQueue;
    Mutex* mMutex;
    int mFps;
    //TimerEvent* mTimerEvent;
    TimeEventManager* mTimerEvent;
};

#endif //_MEDIA_SOURCE_H_