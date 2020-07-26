#include "Env.h"


Env::Env()
{
    mTimerEventQueue = new TimerEventQueue();
}

Env::~Env()
{
    delete mTimerEventQueue;
    mTimerEventQueue = nullptr;
}
void Env::start(){
    std::thread taskThread(run,mTimerEventQueue);
    taskThread.detach();
    return;
}
TimerId Env::addTimer(const TimerEvent& event, uint32_t ms, bool repeat,void*pUser){
    return mTimerEventQueue->addTimer(event,ms,repeat,pUser);
}
void Env::run(void* param){
    TimerEventQueue* timeEventQueue = (TimerEventQueue*)param;
	int64_t iTimeRemain = timeEventQueue->getTimeRemaining();
	while(iTimeRemain >= 0)
	{
		TimeEventManager::mSleep(iTimeRemain);
		timeEventQueue->handleTimerEvent();
		iTimeRemain = timeEventQueue->getTimeRemaining();
    }
    LOGI("Exit task");
    return;
}