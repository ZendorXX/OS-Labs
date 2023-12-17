#include <stdio.h>
#include <dlfcn.h>

void menu() {
    printf("Choose command:\n");
    printf("0 - change realistation of functions\n");
    printf("1 - get count of prime numbers on a segment from A to B\n");
    printf("2 - calculate value of pi with precision K\n");
}

int realistation = 1;
void *lib = NULL;

int (*prime_count)(int, int);
float (*pi)(int);

void load() {
    if (realistation == 1) {
        lib = dlopen("./libreal1.so", RTLD_LAZY);
    } else {
        lib = dlopen("./libreal2.so", RTLD_LAZY);
    }

    if (lib == NULL) {
        perror("dlopen");
    }

    prime_count = dlsym(lib, "prime_count");
    pi = dlsym(lib, "pi");
}

int main() {
    menu();

    int cmd;

    load();

    while (scanf("%d", &cmd)) {
        if (cmd == 0) {
            realistation = (realistation == 1 ? 0 : 1);
            dlclose(lib);
            load();
        }
        else if (cmd == 1) {
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