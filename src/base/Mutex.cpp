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
