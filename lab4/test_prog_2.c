#include <stdio.h>

void menu() {
    printf("Choose command:\n");
    printf("0 - change realistation of functions\n");
    printf("1 - get count of prime numbers on a segment from A to B\n");
    printf("2 - calculate value of pi with precision K\n");
}

int main() {
    int cmd;
    while (scanf("%d", &cmd)) {
        
        menu();
    }
    return 0;
}