#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void handle_sigxcpu(int sig) {
    const char *msg = "\n[SIGNAL] SIGXCPU: CPU time limit exceeded!\n";
    write(STDOUT_FILENO, msg, strlen(msg));
    _exit(EXIT_FAILURE);
}

void draw_numbers(int count, int max) {
    int numbers[count];
    for (int i = 0; i < count; i++) {
        int unique;
        do {
            unique = 1;
            numbers[i] = (rand() % max) + 1;
            for (int j = 0; j < i; j++) {
                if (numbers[i] == numbers[j]) {
                    unique = 0;
                    break;
                }
            }
        } while (!unique);
    }

    printf("Draw %d of %d: ", count, max);
    for (int i = 0; i < count; i++) printf("%d ", numbers[i]);
    printf("\n");
}

int main() {
    struct rlimit limit;
    srand(time(NULL));

    signal(SIGXCPU, handle_sigxcpu);

    limit.rlim_cur = 1; 
    limit.rlim_max = 2;

    if (setrlimit(RLIMIT_CPU, &limit) != 0) {
        perror("setrlimit error");
        return 1;
    }

    printf("CPU time limit set to %ld second. Starting lottery simulation...\n", limit.rlim_cur);

    unsigned long iteration = 0;
    while (1) {
        iteration++;
        if (iteration % 100000 == 0) {
            printf("Iteration %lu: ", iteration);
            draw_numbers(7, 49);
            draw_numbers(6, 36);
        }
    }

    return 0;
}