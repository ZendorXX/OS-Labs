#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#define REAL_FILENAME_NAX 255

#define READ  0
#define WRITE 1

int main() {
    //int tmp = FILENAME_MAX;
    printf("Enter a name of file in which result will be printed:\n");
    char* filename = NULL;
    size_t filename_capacity = 0; // the size in bytes of the block of memory pointed to by the first parameter in getline
    getline(&filename, &filename_capacity, stdin); // getline returns bytes count which will be read, if error occurs it returns -1
    filename[strlen(filename) - 1] = '\0'; // removing '\n'
    if (strlen(filename) > REAL_FILENAME_NAX) {
        printf("Name of file too big!\n");
        return -1;
    }

    int out = open(filename, O_APPEND|O_WRONLY);
    if (out == -1) {
        printf("Error occurred with openin file!\n");
        return -1;
    }

    free(filename);

    int parent_to_child[2], child_to_parent[2];
    if (pipe(parent_to_child) == -1 || pipe(child_to_parent) == -1) {
        perror("pipe");
        return -1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    if (pid > 0) {
        // parent
        close(parent_to_child[READ]);
        close(child_to_parent[WRITE]);

        printf("Enter string:\n");
        char* s = NULL;
        size_t s_capacity = 0;
        size_t len = 0; 
        size_t error = -1;

        while ((len = getline(&s, &s_capacity, stdin)) != error) { 
            write(parent_to_child[WRITE], s, strlen(s));
            s_capacity = 0;
        }
        close(parent_to_child[WRITE]);
        free(s);

        char buff;
        while (read(child_to_parent[READ], &buff, sizeof(buff))) {
            printf("%c", buff);
        }
        close(child_to_parent[READ]);

        close(out);
    }
    else {
        // child
        close(parent_to_child[WRITE]);
        close(child_to_parent[READ]);

        dup2(parent_to_child[READ], STDIN_FILENO);
        dup2(out, STDOUT_FILENO);
        dup2(child_to_parent[WRITE], STDERR_FILENO);

        execl("child.out", "child.out", NULL);
    }

    return 0;
}