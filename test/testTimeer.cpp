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
	long iVedioId = 0;
	TimerEventQueue stTimerQueue;
	stTimerQueue.addTimer(TimerEvent2, 6000, false,(void*)iVedioId);
	stTimerQueue.addTimer(TimerEvent3, 1000, false,(void*)iVedioId);

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
	TimerEventQueue stTimerQueue;
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
