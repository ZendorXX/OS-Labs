#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char* text = NULL;
    size_t text_size = 0;
    char* str = NULL;
    size_t str_capacity = 0;
    while (getline(&str, &str_capacity, stdin) != -1) {
        text_size += strlen(str);
        text = realloc(text, sizeof(char) * text_size);
        for (int i = text_size - strlen(str), j = 0; i < text_size; ++i, ++j) {
            text[i] = str[j];
        }
        str_capacity = 0;
    }
    printf("%s", text);
    return 0;
}