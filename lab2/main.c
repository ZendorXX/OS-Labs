#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <limits.h>

#define MAX INT_MAX
#define MIN INT_MIN

typedef struct {
    int *array;
    size_t left, right;
    int min_value, max_value;
} Thread_Arguments;

void *solve(void *args) {
    Thread_Arguments *data = (Thread_Arguments*)args;

    int max_tmp = MIN;
    int min_tmp = MAX;

    for (size_t i = data->left; i < data->right; ++i) {
        if (data->array[i] > max_tmp) {
            max_tmp = data->array[i];
        } 
        if (data->array[i] < min_tmp) {
            min_tmp = data->array[i];
        }
    }

    data->max_value = max_tmp;
    data->min_value = min_tmp;

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Give count of threads as argument.\n");
        return 0;
    }

    FILE *in = fopen("array.txt", "r");

    size_t size = 0;
    fscanf(in, "%ld", &size);

    int threads_count = atoi(argv[1]);

    if (threads_count < 1 || (size_t) threads_count > size) {
        printf("Invalid count of threads.\n");
        return -1;
    }

    int *array = (int*) calloc(size, sizeof(int));
    for (size_t i = 0; i < size; ++i) {
        fscanf(in, "%d", &array[i]);
    }

    pthread_t thread[threads_count];
    Thread_Arguments args[threads_count];

    size_t step = size / (size_t) threads_count;

    clock_t time = clock();
    for (int i = 0; i < threads_count; ++i) {
        args[i].array = array;
        args[i].left = step * i;
        args[i].right = (i == threads_count - 1) ? size : step * (i + 1);
        args[i].max_value = MIN;
        args[i].min_value = MAX;

        if ( pthread_create(&thread[i], NULL, solve, &args[i]) ) {
            perror("pthread_create");
            return -1;
        }
    }

    for (int i = 0; i < threads_count; ++i) {
        if ( pthread_join(thread[i], NULL) ) {
            perror("pthread_join");
            return -1;
        }
    }

    int max_total = MIN;
    int min_total = MAX;

    for (size_t i = 0; i < (size_t)threads_count; ++i) {    
        if (args[i].max_value > max_total) {
            max_total = args[i].max_value;
        } 
        if (args[i].min_value < min_total) {
            min_total = args[i].min_value;
        }
    }
    time = clock() - time;

    printf("%.10f\n", ((double) time) / CLOCKS_PER_SEC);
    printf("Min: %d, max: %d\n", min_total, max_total);

    free(array);
    fclose(in);
    return 0;
}