
#ifndef   _SIGNAL_H__ 
#define   _SIGNAL_H__
#include <thread>
#include "string"
#include <functional>
#include "Mutex.h"
typedef std::function<void(void*)> SignlCB;

class SignalEvent
{
public:
    enum{
        NONE,
        DONOW,
        STOP,
    };
    SignalEvent();
    ~SignalEvent();
    void start();
    int getSignal();
    void setSignal();
    void unsetSignal();
    void stop();
    void handleTask();
    static void run(void* param);
    void setCallBack(const SignlCB& Callback,void* arg);
    
private:
    int mSignal;
    SignlCB signalCallback = [](void*){};
    void * mParam;
    Mutex* mMutex; 

};
#endif