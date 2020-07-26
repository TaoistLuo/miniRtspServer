#ifndef _ENV_H__
#define _ENV_H__
#include "SignalEvent.h"
#include "TimeEventManager.h"
#include "Mylog.h"
class Env
{    
public:
    Env();
    ~Env();
    void start();
    TimerId addTimer(const TimerEvent& event, uint32_t ms, bool repeat,void*pUser);
    static void run(void* param);
private:
    TimerEventQueue* mTimerEventQueue;
};



#endif