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
    size_t size = 0;
    scanf("%ld", &size);
    int* array = (int*) calloc(size, sizeof(int));
    for (size_t i = 0; i < size; ++i) {
        scanf("%d", &array[i]);
    }
    printf("%d", min(array, size));
    printf("\n");
    free(array);
    return 0;
}