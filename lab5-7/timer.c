#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef struct {
    int status;
    struct timeval start_time, stop_time;
} timer;

timer* init() {
    timer *newT = (timer*)malloc(sizeof(timer));
    newT->status = 0;

    return newT;
}

void startTimer(timer *t) {
    if (t->status == 1) {
        printf("Ошибка: таймер уже запущен\n");
        return;
    }
    gettimeofday(&t->start_time, NULL);
    t->status = 1;
    printf("Таймер запущен\n");
}

void stopTimer(timer *t) {
    if (t->status == 0) {
        printf("Ошибка: таймер уже остановлен\n");
        return;
    }
    gettimeofday(&t->stop_time, NULL);
    t->status = 0;
    printf("Таймер остановлен\n");
}

double getTime(timer *t) {
    struct timeval current_time;
    gettimeofday(&current_time, NULL);

    long seconds = current_time.tv_sec - t->start_time.tv_sec;
    long microseconds = current_time.tv_usec - t->start_time.tv_usec;
    double elapsed_time = seconds + microseconds / 1000000.0;
    return elapsed_time * 1000;
}

int main() {
    timer *t = init();

    char cmd[10];

    while(1) {
        printf("Введите команду (start, stop, time): ");
        scanf("%s", cmd);

        if (strcmp(cmd, "start") == 0) {
            startTimer(t);
        }
        else if (strcmp(cmd, "stop") == 0) {
            stopTimer(t);
        }
        else if (strcmp(cmd, "time") == 0) {
            if (t->status) {
                printf("Текущее прошедшее время: %.3f миллисекунд\n", getTime(t));
            }
            else {
                printf("Таймер остановлен\n");
            }
        }
    }
    return 0;
}