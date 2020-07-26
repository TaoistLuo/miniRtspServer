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