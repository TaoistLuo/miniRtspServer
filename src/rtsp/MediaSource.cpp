#include "MediaSource.h"
#include "Mylog.h"
MediaSource::MediaSource(Env* env):mEnv(env)
{
    mMutex = Mutex::createNew();
    for(int i = 0; i < DEFAULT_FRAME_NUM; ++i){
        mAVFrameInputQueue.push(&mAVFrames[i]);
    }
    setFps(25);
    uint32_t frameTime  = 1000/25;
    mEnv->addTimer(taskCallback,frameTime,true,this);
}

MediaSource::~MediaSource()
{
}

AVFrame* MediaSource::getFrame()
{
    AutoLock mAutoLock(mMutex);
    if(mAVFrameOutputQueue.empty())
    {
        LOGI("mAVFrameOutputQueue was NULL\n");
        return NULL;
    }
    AVFrame* frame = mAVFrameOutputQueue.front();    
    mAVFrameOutputQueue.pop();
    return frame;
}

void MediaSource::putFrame(AVFrame* frame)
{
    AutoLock mAutoLock(mMutex);
    mAVFrameInputQueue.push(frame);
}


void MediaSource::taskCallback(void* arg)
{
    MediaSource* source = (MediaSource*)arg;
    source->readFrame();
}
void MediaSource::start(uint32_t ms)
{
    //mTimerEvent->setTimeout((uint32_t)ms);
    mTimerEvent->start(ms*1000, true);
}