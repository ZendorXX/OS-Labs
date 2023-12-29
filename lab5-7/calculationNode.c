#include <stdio.h>
#include <zmq.h>
#include <assert.h>

#include "timer.h"

int main(int argc, char *argv[]) {
    void *context = zmq_ctx_new();
    void *node_socket = zmq_socket(context, ZMQ_REP);
    int rc = zmq_bind(node_socket, argv[1]);
    assert(rc == 0);

    while(1) {
        printf("I'm in node\n");
        char buffer[10];
        zmq_recv(node_socket, buffer, 10, 0);
        printf("Received: %s\n", buffer);
        zmq_send(node_socket, "Ok", 2, 0);
    }

    /*int run = 1;

    while (run) {
        char buffer[256];
        zmq_recv(node_socket, buffer, 10, 0);
        sleep(1);
        zmq_send(node_socket, "Done", 4, 0);
    }*/
    /*char *cmd = argv[1];
    
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
    }*/

    return 0;
}