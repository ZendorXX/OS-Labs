#include <iostream>
#include <string>
#include <map>

#include "timer.h"

#include <chrono>
using namespace std::chrono_literals;

#include <zmq.hpp>


std::map<std::string, int> var_map;
bool waiting = true;


std::string next(std::string* s) {
    size_t i = 0;
    for (; i < s->size() && (*s)[i] != ' ' && (*s)[i] != '\n'; ++i) {}

    std::string result = s->substr(0, i);
    *s = s->substr(i + 1, s->size() - (i + 1));

    return result;
}

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage:\n\t" << argv[0] << " <socket_path>\n";
        return 0;
    }

    zmq::context_t ctx;
    zmq::socket_t socket(ctx, ZMQ_REP);
    socket.bind(argv[1]);

    Timer *t = createTimer();

    while (true) {
        zmq::message_t request;
        if (socket.recv(request, zmq::recv_flags::none) != -1) {
            std::cout << "Error: zmq_recv\n";
            exit(1);
        }

        std::string s = std::string(static_cast<char*>(request.data()), request.size());
        std::string command = next(&s);

        std::string to_send;

        if (command == "start") {
            startTimer(t);
            to_send = "Ok: start";
        } else if (command == "stop") {
            stopTimer(t);
            to_send = "Ok: stop";
        } else if (command == "time") {
            long time = showTime(t);
            to_send = "Ok: " + std::to_string(time);
        }
        else if (isdigit(command[0])) {
            std::string next_num = command;

            zmq::socket_t next_socket = zmq::socket_t(ctx, ZMQ_REQ);
            next_socket.connect("ipc:///tmp/lab5_" + next_num);

            next_socket.send(zmq::buffer(s), zmq::send_flags::none);

            zmq::message_t request;

            auto start = std::chrono::steady_clock::now();

            while (true) {
                if (next_socket.recv(request, zmq::recv_flags::dontwait)) {
                    to_send = std::string(static_cast<char*>(request.data()), request.size());
                    break;
                }

                if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start) >= 1s) {
                    to_send = "Error: 0";
                    break;
                }
            }
        } else if (command == "ping") {
            to_send = "Ok: 1";
        } else {
            break;
        }

        socket.send(zmq::buffer(to_send), zmq::send_flags::none);
    }

    return 0;
}
