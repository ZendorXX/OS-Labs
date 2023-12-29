#include "timer.h"

Timer* createTimer() {
    Timer *newTimer = (Timer*)malloc(sizeof(Timer));
    newTimer->is_running = 0;
    newTimer->elapsed_time = 0;
    return newTimer;
}

long getCurrentTime() {
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    return (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
}

void startTimer(Timer* timer) {
    if (timer != NULL && !timer->is_running) {
        timer->is_running = 1;
        gettimeofday(&timer->start_time, NULL);
        //printf("Ok: start\n");
    } else {
        printf("Ошибка: недопустимый идентификатор таймера или таймер уже запущен\n");
    }
}

void stopTimer(Timer* timer) {
    if (timer != NULL && timer->is_running) {
        timer->is_running = 0;
        long elapsed_time = getCurrentTime() - (timer->start_time.tv_sec * 1000 + timer->start_time.tv_usec / 1000);
        timer->elapsed_time += elapsed_time;
        //printf("Ok: stop\n");
    } else {
        printf("Ошибка: недопустимый идентификатор таймера или таймер уже остановлен\n");
    }
}

long showTime(Timer* timer) {
    if (timer != NULL) {
        long elapsed_time = timer->elapsed_time;

        if (timer->is_running) {
            elapsed_time += getCurrentTime() - (timer->start_time.tv_sec * 1000 + timer->start_time.tv_usec / 1000);
        }
        return elapsed_time;
        //printf("Ok: time: %ld\n", elapsed_time);
    } else {
        return -1;
    }
}