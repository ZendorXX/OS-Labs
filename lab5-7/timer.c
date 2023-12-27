#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

typedef struct {
    int is_running;
    struct timeval start_time;
    long elapsed_time;
} Timer;

Timer *createTimer() {
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
        printf("Ok: start\n");
    } else {
        printf("Ошибка: недопустимый идентификатор таймера или таймер уже запущен\n");
    }
}

void stopTimer(Timer* timer) {
    if (timer != NULL && timer->is_running) {
        timer->is_running = 0;
        long elapsed_time = getCurrentTime() - (timer->start_time.tv_sec * 1000 + timer->start_time.tv_usec / 1000);
        timer->elapsed_time += elapsed_time;
        printf("Ok: stop\n");
    } else {
        printf("Ошибка: недопустимый идентификатор таймера или таймер уже остановлен\n");
    }
}

void showTime(Timer* timer) {
    if (timer != NULL) {
        long elapsed_time = timer->elapsed_time;

        if (timer->is_running) {
            elapsed_time += getCurrentTime() - (timer->start_time.tv_sec * 1000 + timer->start_time.tv_usec / 1000);
        }

        printf("Ok: time: %ld\n", elapsed_time);
    } else {
        printf("Ошибка: недопустимый идентификатор таймера\n");
    }
}

int main() {
    Timer *timer = createTimer();
    char command[10];

    while (1) {
        printf("> ");
        scanf("%s", command);

        if (strcmp(command, "exec") == 0) {
            scanf("%s", command);

            if (strcmp(command, "start") == 0) {
                startTimer(timer);
            } else if (strcmp(command, "stop") == 0) {
                stopTimer(timer);
            } else if (strcmp(command, "time") == 0) {
                showTime(timer);
            } else {
                printf("Ошибка: недопустимая команда\n");
            }
        } else {
            printf("Ошибка: недопустимая команда\n");
        }
    }

    return 0;
}
