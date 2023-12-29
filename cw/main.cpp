#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include "parser.h"
#include "graph.h"

using namespace std;

#define READ  0
#define WRITE 1

map<int, string> CreateDictionary(vector<Configuration> configs) {
    map<int, string> paths_dictionary;
    for (auto config: configs) {
        paths_dictionary[config.id] = config.path;
    }
    return paths_dictionary;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Error: Not input file!\n";
        exit(1); 
    }

    vector<Configuration> configs = Parser(argv[1]);

    Graph dag_jobs = CreateGraph(configs);
    if (dag_jobs.isEmpty()) {
        cout << "The graph is empty!\n";
        exit(2);
    }

    //dag_jobs.print();

    vector<int> visited(dag_jobs.getSize(), 0);

    for (auto end_job: dag_jobs.end_jobs_id) {
        if (IsCycle(end_job, dag_jobs, visited)) {
            cout << "Cycle!\n";
            exit(3);
        }
    }

    vector<int> traveled(dag_jobs.getSize(), 0);
    vector<int> path = BFS(dag_jobs, traveled);

    for (int i = 0; i < path.size(); ++i) {
        cout << path[i] << ' ';
    }
    cout << '\n';

    map<int, string> job_paths = CreateDictionary(configs);


    int string_size = path.size() + 1;
    char state[string_size];
    for (int i = 0; i < string_size; ++i) {
        state[i] = '0';
    }

    int max_processes = 4;
    int current_processes = 0;

    for (int id = path.size() - 1; id >= 0; --id) {
        while (current_processes >= max_processes) {
            int status;
            pid_t done = wait(&status);
            if (done > 0) { 
                current_processes--; 
                if (WIFEXITED(status)) { 
                    int job_id = WEXITSTATUS(status);
                    state[job_id] = '1';
                }
            }
        }

        int pipe_parent[2];
        int pipe_child[2];
        if (pipe(pipe_parent) == -1 || pipe(pipe_child) == -1) {
            fprintf(stderr, "Pipe Failed");
            exit(1);
        }
        pid_t process_id = fork();
        
        if (process_id == -1) {
            printf("Error: Fork\n");
            exit(1);
        } else if (process_id > 0) {
            close(pipe_parent[READ]);
            write(pipe_parent[WRITE], state, sizeof(char) * (string_size + 1));
            close(pipe_parent[WRITE]);

            ++current_processes;
            
            close(pipe_child[WRITE]);
            read(pipe_child[READ], state, sizeof(char) * (string_size + 1));
            close(pipe_child[READ]);
            
        } else {
            close(pipe_parent[WRITE]);

            if (dup2(pipe_child[WRITE], STDERR_FILENO) == -1) {
                printf("Error: Dup2\n");
                exit(2);
            }

            char received[string_size];
            read(pipe_parent[READ], received, sizeof(char) * (string_size + 1));
            close(pipe_parent[READ]);
            close(pipe_child[READ]);

            string adjacency;
            for (int i = 0; i < dag_jobs.adjacency[path[id]].size(); ++i) {
                adjacency.push_back(dag_jobs.adjacency[path[id]][i] + '0');
            }
            char ID[2];
            sprintf(ID, "%d", path[id]);
            execl(job_paths[path[id]].c_str(), job_paths[path[id]].c_str(), ID, received, adjacency.c_str(), NULL);
            //execl(job_paths[path[id]].c_str(), job_paths[path[id]].c_str(), ID, received, NULL);
        }
    }

     for (int i = 0; i < current_processes; i++) {
        int status;
        pid_t finished = wait(&status);
        if (finished > 0 && WIFEXITED(status)) {
            int job_id = WEXITSTATUS(status);
            state[job_id] = '1';
        }
    }

    for (int id = 1; id < dag_jobs.getSize(); ++id) {
        cout << "Result for job_id_" << id << " is " << state[id] << '\n';
    }

    return 0;
}
