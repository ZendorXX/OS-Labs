#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>

int main() {
    char *filename = "data.txt";
    int out = 0;
    if ((out = open(filename, O_RDWR)) == -1) {
        perror("open");
        exit(1);
    }

    char* text = NULL;
    size_t text_len = 0;

    char* str = NULL;
    size_t str_capacity = 0;

    while (getline(&str, &str_capacity, stdin) != -1) {
        text_len += strlen(str);
        text = realloc(text, sizeof(char) * text_len);
        for (size_t i = text_len - strlen(str), j = 0; i < text_len; ++i, ++j) {
            text[i] = str[j];
        }
        str_capacity = 0;
    }

    char *write_map;
    int write_fd;
    struct stat fileInfo;
    const char *filePath = "./mmapPtoC.txt";
    size_t fileSizeOld, fileSizeNew, textSize;

    if ((write_fd = open(filePath, O_RDWR | O_CREAT, 0664)) == -1) {
        perror("open");
        exit(1);
    }

    if (stat(filePath, &fileInfo) == -1) {
        perror("stat");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    if (pid > 0) {
        // parent
        fileSizeOld = fileInfo.st_size;
        
        textSize = strlen(text);
        fileSizeNew = fileInfo.st_size + textSize;

        if (ftruncate(write_fd, fileSizeNew) == -1) {
            close(write_fd);
            perror("Error resizing the file");
            exit(1);
        }

        write_map = mmap(0, fileSizeNew, PROT_READ | PROT_WRITE, MAP_SHARED, write_fd, 0);
        if (write_map == MAP_FAILED) {
            close(write_fd);
            perror("mmap 1");
            exit(1);
        }
        for (size_t i = 0; i < textSize; i++) {
            write_map[i+fileSizeOld] = text[i];
        }

        if (msync(write_map, fileSizeNew, MS_SYNC) == -1) {
            perror("Could not sync the file to disk");
        }

        if (munmap(write_map, fileSizeNew) == -1) {
            close(write_fd);
            perror("Error un-mmapping the file 1");
            exit(1);
        }
        
        close(write_fd);

        wait(NULL);


    }
    else {
        // child
        dup2(out, STDOUT_FILENO);
        execl("child.out", "child.out", NULL);
    }

    char *read_map;
    int read_fd, offset_in = 0;
    struct stat read_file_info;

    const char *read_file_path = "./mmapCtoP.txt";

    if ((read_fd = open(read_file_path, O_RDWR | O_CREAT, 0664)) == -1) {
        perror("open");
        exit(1);
    }

    if (stat(read_file_path, &read_file_info) == -1) {
        perror("stat");
        exit(1);
    }
    
    read_map = mmap(0, read_file_info.st_size, PROT_READ, MAP_SHARED, read_fd, 0);
    if (read_map == MAP_FAILED) {
        close(read_fd);
        perror("mmap 2");
        exit(1);
    }

    while (offset_in < read_file_info.st_size) {
        printf("%c", read_map[offset_in]);
        offset_in++;
    }
    
    if (munmap(read_map, read_file_info.st_size) == -1) {
        close(read_fd);
        perror("Error un-mmapping the file 2");
        exit(1);
    }
    
    close(read_fd);
    close(out);
}