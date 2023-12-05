#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#define MAX 1e9
#define MIN 1e-9

int *array;
int *max_nums;
int *min_nums;
int thread_no = 0;

void *solve(void *args) {
    size_t *boundes = (size_t*)args;
    size_t left = boundes[0], right = boundes[1];

    int max_value = MIN;
    int min_value = MAX;

    for (size_t i = left; i < right; ++i) {
        if (array[i] > max_value) {
            max_value = array[i];
        } 
        if (array[i] < min_value) {
            min_value = array[i];
        }
    }
    
    max_nums[thread_no] = max_value;
    min_nums[thread_no] = min_value;
    thread_no++;
    

    return NULL;
}

int main() {
    FILE *in = fopen("array.txt", "r");

    size_t size = 0;
    fscanf(in, "%ld", &size);

    array = (int*) calloc(size, sizeof(int));
    max_nums = (int*) calloc(size, sizeof(int));
    min_nums = (int*) calloc(size, sizeof(int));

    for (size_t i = 0; i < size; ++i) {
        fscanf(in, "%d", &array[i]);
    }

    printf("Введите количество потоков: ");
    int threads_count = 1;
    scanf("%d", &threads_count);

    pthread_t thread;
    size_t step = size / threads_count;

    clock_t time = clock();
    for (int i = 0; i < threads_count; ++i) {
        size_t *args = (size_t*)calloc(2, sizeof(size_t));
        args[0] = step * i;
        args[1] = (i == threads_count - 1) ? size : step * (i + 1);
        int err = pthread_create(&thread, NULL, solve, args);
        if (err) {
            perror("pthread_create");
            return -1;
        }

        int err2 = pthread_join(thread, NULL);
        if (err2) {
            perror("pthread_join");
            return -1;
        }
    }
    
    int MAX_value = MIN;
    int MIN_value = MAX;

    for (size_t i = 0; i < (size_t)threads_count; ++i) {    
        if (max_nums[i] > MAX_value) {
            MAX_value = max_nums[i];
        } 
        if (min_nums[i] < MIN_value) {
            MIN_value = min_nums[i];
        }
    }
    time = clock() - time;

    for (size_t i = 0; i < (size_t)threads_count; ++i) {    
        printf("%d ", max_nums[i]);
    }
    printf("\n");
    for (size_t i = 0; i < (size_t)threads_count; ++i) {    
        printf("%d ", min_nums[i]);
    }
    printf("\n");

    printf("%.10f\n", ((double) time) / CLOCKS_PER_SEC);
    printf("Min: %d, max: %d\n", MIN_value, MAX_value);

    free(array);
    fclose(in);
    return 0;
}