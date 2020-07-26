#include <stdio.h>
#include <unistd.h>
#include "TimeEventManager.h"
#include "Mylog.h"
#include "Env.h"
// #define USE2
// #define USE1
#define USE3

#ifdef USE1 //to 
TimeEventManager g_stTimer1;
static int iCnt =0;
void TimerEvent1(void* aa)
{

	LOGI("TimerEvent1 %d execute...\n",iCnt++);
	if(iCnt >= 10)
	{
		g_stTimer1.stop();
	}
}
#endif

#ifdef USE2
void TimerEvent2(void* id)
{

	LOGI("TimerEvent2 execute...\n");
}

void TimerEvent3(void* id)
{

	LOGI("TimerEvent3 execute...\n");
}
#endif


#ifdef USE3
void TimerEventVideo(void*pVideoId)
{
	long iVideoId = (long)pVideoId;
	LOGI("TimerEventVideo execute id:%ld...\n",iVideoId);
}

void TimerEventAudio(void*pAudioId)
{
	long iAudioId = (long)pAudioId;
	LOGI("TimerEventAudio execute id:%ld...\n",iAudioId);
}
void TimerEventtest(void*pAudioId)
{
	long iAudioId = (long)pAudioId;
	LOGI("test..\n");
}

#endif


int main()
{
#ifdef USE1
	//Test loop execution task
    g_stTimer1.setEventCallback(TimerEvent1);
	//单位：us
	int64_t interval_time = 2000000;
	g_stTimer1.start(interval_time, true);
	LOGI("End of Usage 1 \n");
	return 1;
	/////////Usage 1 END
#endif

#ifdef USE2
	//添加两个执行一次的定时器
	long iVedioId = 0;
	TimerEventQueue stTimerQueue;
	stTimerQueue.addTimer(TimerEvent2, 6000, false,(void*)iVedioId);
	stTimerQueue.addTimer(TimerEvent3, 1000, false,(void*)iVedioId);

	//距离最近下一次执行还剩多少时间
	int64_t iTimeRemain = stTimerQueue.getTimeRemaining();
	while(iTimeRemain>0)
	{
	    static int i = 0;
		usleep(iTimeRemain*1000);
        stTimerQueue.handleTimerEvent();
		iTimeRemain = stTimerQueue.getTimeRemaining();
	}

	//Usage 2 END

	return 1;
#endif

#ifdef USE3
	//实际应用场景 音视频转发时 交替发送音视频 视频帧率25 40ms发送一帧视频
	//音频采用率44100 AAC 一帧时间(1024*1000)/44100=23ms 23ms发送一帧
	TimerEventQueue stTimerQueue;

	//发送第一帧视频
	LOGI("Send First Video Frame...\n");
	long iVedioId = 0;
	LOGI("Send First Audio Frame...\n");
	long iAudioId = 1;
	Env env;
	env.addTimer(TimerEventVideo, 40, true,(void*)iVedioId);
	env.addTimer(TimerEventAudio, 23, true,(void*)iAudioId);
	env.start();
	TimeEventManager::mSleep(2000);
	LOGI("test add when running");
	env.addTimer(TimerEventtest, 40, false,(void*)iAudioId);
	while (1){
		LOGI("in main");
		TimeEventManager::mSleep(500);
	}
	return 1;

#endif

}
