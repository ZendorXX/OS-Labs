#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

typedef struct {
    int is_running;
    struct timeval start_time;
    long elapsed_time;
} Timer;

Timer* createTimer();

long getCurrentTime();

void startTimer(Timer* timer);

void stopTimer(Timer* timer);

void showTime(Timer* timer);

#endif