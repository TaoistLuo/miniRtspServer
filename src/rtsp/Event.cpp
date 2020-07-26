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