#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE* out = fopen("array.txt", "w");

    size_t size = 0;
    scanf("%ld", &size);
    fprintf(out, "%ld\n", size);
    for (size_t i = 0; i < size; ++i) {
        int value = rand() % 1000 + 1;
        fprintf(out, "%d ", value);
    }

    fclose(out);

    return 0;
}