#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main() {
    struct rlimit limit;
    int process_count = 0;

    limit.rlim_cur = 100; 
    limit.rlim_max = 200;

    if (setrlimit(RLIMIT_NPROC, &limit) != 0) {
        perror("setrlimit error");
        return 1;
    }

    printf("Max processes limit set to %ld. Starting fork bomb simulation...\n", limit.rlim_cur);

    while (1) {
        pid_t pid = fork();

        if (pid < 0) {
            if (errno == EAGAIN) {
                printf("\n[LIMIT REACHED] Cannot create more processes. Total created in this loop: %d\n", process_count);
            } else {
                perror("\nFork error");
            }
            break;
        } else if (pid == 0) {
            sleep(10);
            exit(0);
        } else {
            process_count++;
            if (process_count % 5 == 0) {
                printf("Processes created: %d\n", process_count);
            }
        }
    }

    printf("Cleaning up child processes...\n");
    return 0;
}