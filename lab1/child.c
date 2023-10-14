#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    char* s = NULL;
    size_t capacity = 0; // the size in bytes of the block of memory pointed to by the first parameter in getline
    size_t len = 0; 
    size_t error = -1;
    // getline returns bytes count which will be read, if error occurs it returns -1
    int count_of_strings = 0;
    while ((len = getline(&s, &capacity, stdin)) != error) { 
        ++count_of_strings;

        s[strlen(s) - 1] = '\0'; // removing '\n'
        char last_symbol =  s[strlen(s) - 1];
        if (last_symbol == '.' || last_symbol == ';') {
            printf("%s\n", s);
        } 
        else {
            fprintf(stderr, "%d string isn't correct\n", count_of_strings);
        }

        capacity = 0;
    }
    free(s);
    return 0;
}