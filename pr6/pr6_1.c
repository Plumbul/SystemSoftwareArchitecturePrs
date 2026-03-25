#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MEM_SIZE (100 * 1024 * 1024)

int main(int argc, char *argv[]) {
    char *buffer = malloc(MEM_SIZE);
    if (!buffer) return 1;
    memset(buffer, 'A', MEM_SIZE);

    printf("Parent PID: %d. Memory initialized.\n", getpid());

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        if (argc > 1 && strcmp(argv[1], "write") == 0) {
            printf("Child (PID: %d): Modifying memory (triggering CoW)...\n", getpid());
            for (int i = 0; i < MEM_SIZE; i += 4096) {
                buffer[i] = 'B';
            }
        } else {
            printf("Child (PID: %d): Only reading memory (CoW not triggered)...\n", getpid());
            char sum = 0;
            for (int i = 0; i < MEM_SIZE; i += 4096) {
                sum += buffer[i];
            }
            (void)sum; 
        }
        free(buffer);
        return 0;
    } else {
        wait(NULL);
        free(buffer);
        printf("Parent: Child finished.\n");
    }

    return 0;
}