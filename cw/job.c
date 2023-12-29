#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Invalid Arguments\n");
        exit(2);
    }

    /*int id = atoi(argv[1]);
    char* state = argv[2];

    printf("id - %d, state - %s\n", id, state);

    write(2, state, sizeof(char*));*/

    sleep(1);
    
    int id = atoi(argv[1]);
    char* state = argv[2];
    char* adjacency = argv[3];
    printf("I receive id - %d, state - %s, adjacency - %s\n", id, state, adjacency);



    int current_sum = 0;
    for (int i = 0; i < strlen(adjacency); ++i) {
        current_sum += state[adjacency[i] - '0'] - '0';
    }
    ++current_sum;

    state[id] = current_sum + '0';
    // printf("State now %s\n", state);
    write(2, state, sizeof(char*));
    return 0;
}