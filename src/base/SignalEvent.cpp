#include "SignalEvent.h"
#include "Mylog.h"
SignalEvent::SignalEvent(){
    mMutex = new Mutex();
    mSignal = 0;
}
SignalEvent::~SignalEvent(){
    mSignal = 0;
    delete mMutex;
}

void SignalEvent::start(){
    std::thread taskThread(run,this);
    taskThread.detach();;
}
int SignalEvent::getSignal(){
    AutoLock Auto(mMutex);
    return mSignal;
}

void SignalEvent::setSignal(){
    AutoLock Auto(mMutex);
    mSignal = 1;
}
void SignalEvent::unsetSignal(){
    AutoLock Auto(mMutex);
    mSignal = 0;
}
void SignalEvent::stop(){
    AutoLock Auto(mMutex);
    mSignal = 2;
}
void SignalEvent::handleTask(){
    if(signalCallback){
        signalCallback(mParam);
    }
    
}
void SignalEvent::run(void* param){
    SignalEvent* signalTask = (SignalEvent*)param;
    int type = 0;
    while(1){
        type = signalTask->getSignal();
        LOGI("sig:%d",type);
        switch (type)
        {
        case NONE:
            std::this_thread::sleep_for(std::chrono::microseconds(200));
            break;
        case DONOW:
            signalTask->handleTask();
            signalTask->unsetSignal();
            std::this_thread::sleep_for(std::chrono::microseconds(200));
            break;
        case STOP:
            return;
            break;
        default:
            std::this_thread::sleep_for(std::chrono::microseconds(200));
            break;
        }
    }


}
void SignalEvent::setCallBack(const SignlCB& Callback,void* arg){
    AutoLock Auto(mMutex);
    mParam = arg;
    signalCallback = Callback;

}