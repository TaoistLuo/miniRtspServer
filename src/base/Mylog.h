#ifndef   _MYLOG_H__ 
#define   _MYLOG_H__
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/syscall.h>
#include <iostream>
#define TAG "RTSP"
//#define LOGI printf
char * time2String();
#define msleep(x) usleep(x*1000)

#define LOGI(format, ...) printf("%s D %s: [%s:%d] "#format "\n", \
time2String(), TAG, __FUNCTION__,__LINE__, ##__VA_ARGS__)
#endif