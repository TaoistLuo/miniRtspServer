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