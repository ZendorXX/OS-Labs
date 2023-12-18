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
    char *map;
    int fd, offset = 0;
    struct stat fileInfo;
    size_t fileSizeOld, fileSizeNew, textSize;

    const char *filePath = "./mmapPtoC.txt";

    if ((fd = open(filePath, O_RDWR | O_CREAT, 0664)) == -1) {
        perror("open");
        exit(1);
    }

    if (stat(filePath, &fileInfo) == -1) {
        perror("stat");
        exit(1);
    }

    if (fileInfo.st_size != 0) {
        map = mmap(0, fileInfo.st_size, PROT_READ, MAP_SHARED, fd, 0);
        if (map == MAP_FAILED) {
            close(fd);
            perror("mmap");
            exit(1);
        }
        while (offset < fileInfo.st_size) {
            printf("%c", map[offset]);
            offset++;
        }
        printf("\n");
        if (munmap(map, fileInfo.st_size) == -1) {
            close(fd);
            perror("Error un-mmapping the file");
            exit(1);
        }
    }

    /*fileSizeOld = fileInfo.st_size;
    
    textSize = strlen(text);
    fileSizeNew = fileInfo.st_size + textSize;

    if (ftruncate(fd, fileSizeNew) == -1) {
        close(fd);
        perror("Error resizing the file");
        exit(1);
    }

    map = mmap(0, fileSizeNew, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        close(fd);
        perror("mmap");
        exit(1);
    }
    for (size_t i = 0; i < textSize; i++) {
        map[i+fileSizeOld] = text[i];
    }

    if (msync(map, fileSizeNew, MS_SYNC) == -1) {
        perror("Could not sync the file to disk");
    }*/

    if (munmap(map, fileInfo.st_size) == -1) {
        close(fd);
        perror("Error un-mmapping the file");
        exit(1);
    }
    
    close(fd);
}