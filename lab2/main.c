#include <stdio.h>
#include <stdlib.h>

#define MAX 1e9
#define MIN 1e-9

int min(int* array, size_t size) {
    int result = MAX;
    for (size_t i = 0; i < size; ++i) {
        if (array[i] < result) {
            result = array[i];
        }
    }
    return result;
}

int max(int* array, size_t size) {
    int result = MIN;
    for (size_t i = 0; i < size; ++i) {
        if (array[i] > result) {
            result = array[i];
        }
    }
    return result;
}

int main() {
    FILE* in = fopen("array.txt", "r");
    size_t size = 0;
    scanf("%ld", &size);
    int* array = (int*) calloc(size, sizeof(int));
    for (size_t i = 0; i < size; ++i) {
        //scanf("%d", &array[i]);
        fscanf(in, "%d", &array[i]);
    }
    for (size_t i = 0; i < 10; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
    printf("min: %d, max: %d\n", min(array, size), max(array, size));

    free(array);
    fclose(in);
    return 0;
}