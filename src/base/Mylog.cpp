#include "Mylog.h"
char * time2String() {
  struct timespec ts;
  clock_gettime( CLOCK_REALTIME, &ts);
  struct tm * timeinfo = localtime(&ts.tv_sec);
  static char timeStr[20];
  sprintf(timeStr, "%.2d-%.2d %.2d:%.2d:%.2d.%.3ld", timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, ts.tv_nsec / 1000000);
  return timeStr;
}