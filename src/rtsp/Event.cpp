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
#include <stdio.h>
#include "Mylog.h"
#include "Event.h"
#include <thread>

TriggerEvent* TriggerEvent::createNew(void* arg)
{
    return new TriggerEvent(arg);
}

TriggerEvent* TriggerEvent::createNew()
{
    return new TriggerEvent(NULL);
}

TriggerEvent::TriggerEvent(void* arg) :
    mArg(arg)
{

}

void TriggerEvent::handleEvent()
{
    if(mTriggerCallback){
        mTriggerCallback(mArg);
    }
}
/*
TimerEvent* TimerEvent::createNew(void* arg)
{
    return new TimerEvent(arg);
}

TimerEvent* TimerEvent::createNew()
{
    return new TimerEvent(NULL);
}

TimerEvent::TimerEvent(void* arg) :
    mArg(arg)
{
    
}

void TimerEvent::handleEvent()
{
    if(mTimeoutCallback){
        mTimeoutCallback(mArg);
    }
        
}
void TimerEvent::doTimerEventLoop(void* arg){
    TimerEvent* event = (TimerEvent*)arg;
    while (1)
    {
        msleep(event->mTimeout);
        event->handleEvent();

    }
}

void TimerEvent::start(){
    std::thread envenTask(doTimerEventLoop,this);
    envenTask.detach();
    LOGI("create TimerEventTask thread ok\n");
    return;
}
void TimerEvent::setTimeout(uint32_t timeOut){
    mTimeout = timeOut;
}
*/
IOEvent* IOEvent::createNew(int fd, void* arg)
{
    if(fd < 0){
        return NULL;
    }
        

    return new IOEvent(fd, arg);
}

IOEvent* IOEvent::createNew(int fd)
{
    if(fd < 0){
        return NULL;
    }
        
    
    return new IOEvent(fd, NULL);
}

IOEvent::IOEvent(int fd, void* arg) :
    mFd(fd),
    mArg(arg),
    mEvent(EVENT_NONE),
    mREvent(EVENT_NONE),
    mReadCallback(NULL),
    mWriteCallback(NULL),
    mErrorCallback(NULL)
{

}

void IOEvent::handleEvent()
{
    if (mReadCallback && (mREvent & EVENT_READ))
    {
        mReadCallback(mArg);
    }

    if (mWriteCallback && (mREvent & EVENT_WRITE))
    {
        mWriteCallback(mArg);
    }
    
    if (mErrorCallback && (mREvent & EVENT_ERROR))
    {
        mErrorCallback(mArg);
    }
}

void IOEvent::doEventLoop(void* arg){
    IOEvent* event = (IOEvent*)arg;
    while (1)
    {
        msleep(2);
        event->handleEvent();

    }
}

void IOEvent::start(){
    std::thread envenTask(doEventLoop,this);
    envenTask.detach();
    LOGI("create envenTask thread ok\n");
    return;
}