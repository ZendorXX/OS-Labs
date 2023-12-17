#include <stdio.h>

#include "library.h"

void menu() {
    printf("Choose function:\n");
    printf("1 - get count of prime numbers on a segment from A to B\n");
    printf("2 - calculate value of pi with precision K\n");
}

int main() {
    int cmd;
    menu();
    while (scanf("%d", &cmd)) {
        if (cmd == 1) {
            int A, B;
            scanf("%d", &A);
            scanf("%d", &B);
            printf("%d\n", prime_count(A, B));
        } 
        else if (cmd == 2) {
            int K;
            scanf("%d", &K);
            printf("%f\n", pi(K));
        }
        else {
            printf("Wrong argument\n");
        }
        menu();
    }
    return 0;
}