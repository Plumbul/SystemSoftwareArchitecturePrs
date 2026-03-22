#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void handle_sigxfsz(int sig) {
    const char *msg = "\nHard file size limit reached!\n";
    write(STDOUT_FILENO, msg, strlen(msg));
    _exit(EXIT_FAILURE); 
}

int main() {
    struct rlimit limit;
    const char *filename = "dice_results.txt";
    int roll;
    
    srand(time(NULL));

    signal(SIGXFSZ, handle_sigxfsz);

    limit.rlim_cur = 100;
    limit.rlim_max = 200;

    if (setrlimit(RLIMIT_FSIZE, &limit) != 0) {
        perror("setrlimit error");
        return 1;
    }

    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        perror("File opening error");
        return 1;
    }

    printf("File size limit set to %ld bytes. Starting simulation...\n", limit.rlim_cur);

    while (1) {
        roll = (rand() % 6) + 1;
        
        if (fprintf(f, "Dice roll result: %d\n", roll) < 0) {
            perror("\nWrite error");
            break;
        }
        
        if (fflush(f) != 0) {
            perror("\nfflush error (limit exceeded)");
            break;
        }
        
        printf("Recorded roll: %d\n", roll);
        usleep(100000);
    }

    fclose(f);
    return 0;
}
