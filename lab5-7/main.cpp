#include <iostream>
#include <unistd.h>
#include <string>
#include <optional>
#include <tuple>
#include <thread>

#include <zmq.hpp>

#include "btree.hpp"

static ProcessTree processes = ProcessTree();

zmq::context_t ctx;
zmq::socket_t root_socket = zmq::socket_t(ctx, ZMQ_REQ);

void send_msg() {
    zmq::message_t request;

    if (root_socket.recv(request, zmq::recv_flags::none) == -1) {
        std::cout << "Error: zmq_recv\n";
        exit(1);
    }

    std::string message = std::string(static_cast<char*>(request.data()), request.size());
    std::cout << message << std::endl;
}

std::vector<std::string> split(std::string s) {
    std::vector<std::string> result;
    std::stringstream ss(s);  
    std::string word;
    while (ss >> word) {
        result.push_back(word);
    }
    return result;
}

int main() {
    root_socket.connect("ipc:///tmp/lab5_0");

    pid_t child_pid = fork();

    if (child_pid == -1) {
        std::cout << "Error: Could not fork current process!\n";
        return 1;
    } else if (child_pid == 0) {
        const char *argv[] = { "./child.out", "ipc:///tmp/lab5_0", NULL };
        if (execv(argv[0], const_cast<char* const*>(argv)) == -1) std::cout << "Error: Could not create a child process!\n";
    }

    processes.insert(std::tuple(0, true));

    std::string line = "";

    std::cout << "> ";
    while (getline(std::cin, line)) {
        line.push_back('\n');

        std::vector<std::string> command = split(line);

        if (command[0] == std::string("create")) {
            int child_id = std::stoi(command[1]);

            if (processes.is_in_tree(child_id)) {
                std::cout << "Error: Already exists\n";
            } else {
                std::string socket_path = "ipc:///tmp/lab5_" + std::to_string(child_id);

                pid_t child_pid = fork();

                if (child_pid == -1) {
                    std::cout << "Error: Fork\n";
                } else if (child_pid == 0) {
                    const char *argv[] = { "./child.out", socket_path.c_str(), NULL };
                    if (execv(argv[0], const_cast<char* const*>(argv)) == -1) std::cout << "Error: Execl\n";
                } else {
                    std::cout << "Ok: " << child_pid << '\n';
                    processes.insert(std::tuple(child_id, true));
                }
            }
        } else if (command[0] == std::string("exec")) {
            int child_id = std::stoi(command[1]);

            std::optional<std::vector<int>> path = processes.get_path_to(child_id);

            if (!path) {
                std::cout << "Error: " << child_id << ": Not found\n";
                std::cout << "> ";
                continue;
            }

            std::string msg_command = command[2] + '\n';
            std::string msg;

            for (unsigned long i = 1; i < path.value().size(); ++i) {
                int id = path.value()[i];
                msg += std::to_string(id) + ' ';
            }
            msg += msg_command;

            root_socket.send(zmq::buffer(msg), zmq::send_flags::none);

            std::thread wait(send_msg);
            wait.join();

        } else if (command[0] == std::string("ping")) {
            int child_id = std::stoi(command[1]);
         
            std::optional<std::vector<int>> path = processes.get_path_to(child_id);

            if (!path) {
                std::cout << "Error: " << child_id << ": Not found\n";
                std::cout << "> ";
                continue;
            }

            std::string msg_command = "ping\n";
            std::string msg;

            for (unsigned long i = 1; i < path.value().size(); ++i) {
                int id = path.value()[i];
                msg += std::to_string(id) + ' ';
            }
            msg += msg_command;

            root_socket.send(zmq::buffer(msg), zmq::send_flags::none);
            
            std::thread wait(send_msg);
            wait.join();
        } else if (command[0] == std::string("print")) {
            processes.print_levels();
        } else if (command[0] == std::string("exit")) {
            break;
        }

        std::cout << "> ";
    }

    return 0;
}