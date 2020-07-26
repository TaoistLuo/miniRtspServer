#include "Mutex.h"
#include "Mylog.h"
Mutex* Mutex::createNew()
{
    return new Mutex();
}

Mutex::Mutex()
{
    pthread_mutex_init(&mMutex, NULL);
}

Mutex::~Mutex()
{
    pthread_mutex_destroy(&mMutex);
}

void Mutex::lock()
{
    pthread_mutex_lock(&mMutex);
}

void Mutex::unlock()
{
    pthread_mutex_unlock(&mMutex);
}

AutoLock::AutoLock(Mutex* mutex)
{
    if(!mutex){
        LOGI("lock failed the input was NULL \n");
        return;
    }
    mMutex=mutex;
    mMutex->lock();
}

AutoLock::~AutoLock()
{
    mMutex->unlock();
}
