#ifndef _MUTEX_H__
#define _MUTEX_H__
#include <pthread.h>

class Mutex
{
public:
    static Mutex* createNew();
    
    Mutex();
    ~Mutex();

    void lock();
    void unlock();
    
    pthread_mutex_t* get() { return &mMutex; };

private:
    pthread_mutex_t mMutex;

};

class AutoLock
{
public:
    AutoLock(Mutex* mutex);
    ~AutoLock();

private:
    Mutex* mMutex;

};

#endif //_MUTEX_H__