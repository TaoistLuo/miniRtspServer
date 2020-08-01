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
#include <iostream>
#include "TimeEventManager.h"



using namespace std;
using namespace std::chrono;

TimerId TimerEventQueue::addTimer(const TimerEvent& event, uint32_t ms, bool repeat,void*pUser)
{
    std::lock_guard<std::mutex> locker(_mutex);

    int64_t timeoutPoint = getTimeNow();
    TimerId timerId = {timeoutPoint+ms, ++_lastTimerId};
    auto timer = make_shared<TimeEventManager>(event, ms, repeat,pUser);

    timer->setNextTimeout(timeoutPoint);

    if(repeat)
    {
        _repeatTimers.emplace(timerId.second, timer);
    }

    _timers.insert(std::pair<TimerId,std::shared_ptr<TimeEventManager>>(timerId, std::move(timer)));

    return timerId;
}

void TimerEventQueue::removeTimer(TimerId timerId)
{
    std::lock_guard<std::mutex> locker(_mutex);

    auto iter = _repeatTimers.find(timerId.second);	
    if(iter != _repeatTimers.end())
    {
        TimerId t = {iter->second->getNextTimeout(), timerId.second};
        _repeatTimers.erase(iter);
        _timers.erase(t);		
    }
    else
    {
        _timers.erase(timerId);
    }
}

int64_t TimerEventQueue::getTimeNow()
{	
    auto timePoint = steady_clock::now();	
    return duration_cast<milliseconds>(timePoint.time_since_epoch()).count();	
}

int64_t TimerEventQueue::getTimeRemaining()
{	
    std::lock_guard<std::mutex> locker(_mutex);

    if(_timers.empty()){
        return -1;
    }
     
    int64_t ms = _timers.begin()->first.first - getTimeNow();
    if(ms <= 0){
        return 0;
    }
    return ms;
}

void TimerEventQueue::handleTimerEvent()
{
    if(!_timers.empty() || !_repeatTimers.empty())
    {
        std::lock_guard<std::mutex> locker(_mutex);

        int64_t timePoint = getTimeNow();
        while(!_timers.empty() && _timers.begin()->first.first<=timePoint)
        {	
            _timers.begin()->second->eventCallback(_timers.begin()->second->_pUser);
            if(_timers.begin()->second->isRepeat())
            {
                _timers.begin()->second->setNextTimeout(timePoint);
                TimerId t = {_timers.begin()->second->getNextTimeout(), _timers.begin()->first.second};		
                auto timerPtr = std::move(_timers.begin()->second);			
                _timers.erase(_timers.begin());
                _timers.insert(std::pair<TimerId,std::shared_ptr<TimeEventManager>>(t, std::move(timerPtr)));

                //_timers.insert(std::pair<TimerId,std::shared_ptr<TimeEventManager>>(timerId, std::move(timer)));
            }
            else		
            {
                _timers.erase(_timers.begin());
            }
        }	
    }
}

