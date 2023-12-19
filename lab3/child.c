#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>

int main() {
    char *map_in;
    int fd_in, offset = 0;
    struct stat fileInInfo;

    const char *filePath = "./mmapPtoC.txt";

    if ((fd_in = open(filePath, O_RDWR | O_CREAT, 0664)) == -1) {
        perror("open");
        exit(1);
    }

    if (stat(filePath, &fileInInfo) == -1) {
        perror("stat");
        exit(1);
    }

    char *text = NULL;
    size_t text_len = 0;

    if (fileInInfo.st_size != 0) {
        map_in = mmap(0, fileInInfo.st_size, PROT_READ, MAP_SHARED, fd_in, 0);
        if (map_in == MAP_FAILED) {
            close(fd_in);
            perror("mmap 3");
            exit(1);
        }
        int start = 0;
        int count_of_strings = 0;

        while (offset < fileInInfo.st_size) {
            if (map_in[offset] == '\n') {
                ++count_of_strings;
                
                if (map_in[offset - 1] == '.' || map_in[offset - 1] == ';') {
                    for (int i = start; i < offset + 1; ++i) {
                        printf("%c", map_in[i]);
                    }
                } 
                else {
                    char *template = " string isn't correct\n";

                    int number_lenght = snprintf(NULL, 0, "%d", count_of_strings);
                    char *number_to_str = malloc(number_lenght + 1);
                    snprintf(number_to_str, number_lenght + 1, "%d", count_of_strings);

                    char *string_responce = malloc(strlen(number_to_str) + strlen(template) + 1);
                    strcpy(string_responce, number_to_str);
                    strcat(string_responce, template);

                    text_len += strlen(string_responce);
                    text = realloc(text, sizeof(char) * text_len);

                    for (size_t i = text_len - strlen(string_responce), j = 0; i < text_len; ++i, ++j) {
                        text[i] = string_responce[j];
                    }
                }
                start = offset + 1;
            }
            ++offset;
        }
        printf("\n");

        if (munmap(map_in, fileInInfo.st_size) == -1) {
            close(fd_in);
            perror("Error un-mmap_inping the file 3");
            exit(1);
        }
    }

    close(fd_in);
    
    char *map_out;
    int fd_out = 0;
    struct stat fileOutInfo;
    size_t fileOutSizeOld, fileOutSizeNew, textOutSize;

    const char *fileOutPath = "./mmapCtoP.txt";

    if ((fd_out = open(fileOutPath, O_RDWR | O_CREAT, 0664)) == -1) {
        perror("open");
        exit(1);
    }

    if (stat(fileOutPath, &fileOutInfo) == -1) {
        perror("stat");
        exit(1);
    }

    fileOutSizeOld = fileOutInfo.st_size;

    textOutSize = strlen(text);
    fileOutSizeNew = fileOutInfo.st_size + textOutSize;

    if (ftruncate(fd_out, fileOutSizeNew) == -1) {
        close(fd_out);
        perror("Error resizing the file 4");
        exit(1);
    }

    map_out = mmap(0, fileOutSizeNew, PROT_READ | PROT_WRITE, MAP_SHARED, fd_out, 0);
    if (map_out == MAP_FAILED) {
        close(fd_out);
        perror("mmap 4");
        exit(1);
    }
    for (size_t i = 0; i < textOutSize; i++) {
        map_out[i+fileOutSizeOld] = text[i];
    }

    if (msync(map_out, fileOutSizeNew, MS_SYNC) == -1) {
        perror("Could not sync the file to disk");
    }

    if (munmap(map_out, fileOutSizeNew) == -1) {
        close(fd_out);
        perror("Error un-mmapping the file");
        exit(1);
    }
    
    close(fd_out);
}