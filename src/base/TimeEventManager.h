#ifndef _TIMEEVENTMANAGER_H__
#define _TIMEEVENTMANAGER_H__

#include <map>
#include <unordered_map>
#include <chrono>
#include <functional>
#include <cstdint>
#include <chrono>
#include <memory>
#include <mutex>
#include <thread>

    
typedef std::function<void(void*)> TimerEvent;
typedef std::pair<int64_t, uint32_t> TimerId;

class TimeEventManager
{
public:
    TimeEventManager(const TimerEvent& event, uint32_t ms, bool repeat,void*pUser)
        : eventCallback(event)
        , _interval(ms)
        , _isRepeat(repeat)
    	,_pUser(pUser)
    {
        if (_interval == 0)
            _interval = 1;
    }

	TimeEventManager() { }

	//get the type of timer
    bool isRepeat() const 
    { 
        return _isRepeat;
    }

    static void mSleep(unsigned ms) 
    { 
        std::this_thread::sleep_for(std::chrono::milliseconds(ms)); 
    }

	void setEventCallback(const TimerEvent& event)
	{
		eventCallback = event;
	}
	void setEventCallback(const TimerEvent& event,void* arg)
	{
		eventCallback = event;
		_pUser = arg;
	}
	//start the timer
	void start(int64_t microseconds, bool repeat=false)
	{
		_isRepeat = repeat;
		auto timeBegin = std::chrono::high_resolution_clock::now();
		int64_t elapsed = 0;

		do
		{
			std::this_thread::sleep_for(std::chrono::microseconds(microseconds - elapsed));
			timeBegin = std::chrono::high_resolution_clock::now();
			eventCallback(_pUser);
			elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - timeBegin).count();
			if (elapsed < 0){
                 elapsed = 0;
			}
		} while (_isRepeat);
	}

	void stop()
	{
		_isRepeat = false;
	}

private:
	friend class TimerEventQueue;

	void setNextTimeout(int64_t currentTimePoint)
	{
		_nextTimeout = currentTimePoint + _interval;
	}

	int64_t getNextTimeout() const
	{
		return _nextTimeout;
	}
	TimerEvent eventCallback = [](void*){};
    bool _isRepeat = false;
    uint32_t _interval = 0;
    int64_t _nextTimeout = 0;
    void * _pUser;
};

class TimerEventQueue
{
public:
    TimerId addTimer(const TimerEvent& event, uint32_t ms, bool repeat,void*pUser);
    void removeTimer(TimerId timerId);

    // return the latest time of timeout,is not ,return -1
    int64_t getTimeRemaining();
    void handleTimerEvent();

private:
    int64_t getTimeNow();

    std::mutex _mutex;
    std::map<TimerId, std::shared_ptr<TimeEventManager>> _timers;
    std::unordered_map<uint32_t, std::shared_ptr<TimeEventManager>> _repeatTimers;
    uint32_t _lastTimerId = 0;
    uint32_t _timeRemaining = 0;
};


#endif //end _TIMEEVENTMANAGER_H__



