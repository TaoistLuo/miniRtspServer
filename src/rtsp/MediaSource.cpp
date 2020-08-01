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