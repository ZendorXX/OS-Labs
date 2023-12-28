#include <stdio.h>

#include "timer.h"

int main(int argc, char *argv[]) {
    char *cmd = argv[1];
    
    Timer *t = createTimer();

    if (strcmp(cmd, "start") == 0) {
        startTimer(t);
    }
    else if (strcmp(cmd, "stop") == 0) {
        stopTimer(t);
    }
    else if (strcmp(cmd, "time") == 0) {
        showTime(t);
    }
    else {
        printf("Unknown command\n");
    }

    return 0;
}