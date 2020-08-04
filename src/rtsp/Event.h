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
#ifndef _EVENT_H_
#define _EVENT_H_

//#include "base/Logging.h"

typedef void (*EventCallback)(void*);

class TriggerEvent
{
public:
    static TriggerEvent* createNew(void* arg);
    static TriggerEvent* createNew();

    TriggerEvent(void* arg);
    ~TriggerEvent() {  };

    void setArg(void* arg) { mArg = arg; }
    void setTriggerCallback(EventCallback cb) { mTriggerCallback = cb; }
    void handleEvent();

private:
    void* mArg;
    EventCallback mTriggerCallback;
};
/*
class TimerEvent
{
public:
    static TimerEvent* createNew(void* arg);
    static TimerEvent* createNew();

    TimerEvent(void* arg);
    ~TimerEvent() { }

    void setArg(void* arg) { mArg = arg; }
    void setTimeoutCallback(EventCallback cb) { mTimeoutCallback = cb; }
    void handleEvent();
    static void doTimerEventLoop(void* arg);
    void start();
    void setTimeout(uint32_t timeOut);

private:
    void* mArg;
    uint32_t mTimeout;
    EventCallback mTimeoutCallback;
};
*/
class IOEvent
{
public:
    enum IOEventType
    {
        EVENT_NONE = 0,
        EVENT_READ = 1,
        EVENT_WRITE = 2,
        EVENT_ERROR = 4,
    };
    
    static IOEvent* createNew(int fd, void* arg);
    static IOEvent* createNew(int fd);

    IOEvent(int fd, void* arg);
    ~IOEvent() { }

    int getFd() const { return mFd; }
    int getEvent() const { return mEvent; }
    void setREvent(int event) { mREvent = event; }
    void setArg(void* arg) { mArg = arg; }

    void setReadCallback(EventCallback cb) { mReadCallback = cb; };
    void setWriteCallback(EventCallback cb) { mWriteCallback = cb; };
    void setErrorCallback(EventCallback cb) { mErrorCallback = cb; };

    void enableReadHandling() { mEvent |= EVENT_READ; }
    void enableWriteHandling() { mEvent |= EVENT_WRITE; }
    void enableErrorHandling() { mEvent |= EVENT_ERROR; }
    void disableReadeHandling() { mEvent &= ~EVENT_READ; }
    void disableWriteHandling() { mEvent &= ~EVENT_WRITE; }
    void disableErrorHandling() { mEvent &= ~EVENT_ERROR; }

    bool isNoneHandling() const { return mEvent == EVENT_NONE; }
    bool isReadHandling() const { return (mEvent & EVENT_READ) != 0; }
    bool isWriteHandling() const { return (mEvent & EVENT_WRITE) != 0; }
    bool isErrorHandling() const { return (mEvent & EVENT_ERROR) != 0; };

    void handleEvent();
    static void doEventLoop(void* arg);
    void start();
private:
    int mFd;
    void* mArg;
    int mEvent;
    int mREvent;
    EventCallback mReadCallback;
    EventCallback mWriteCallback;
    EventCallback mErrorCallback;
};
#endif //_EVENT_H_