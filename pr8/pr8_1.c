#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int pipefd[2];
    char buffer[100000];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    fcntl(pipefd[1], F_SETFL, O_NONBLOCK);

    int count = write(pipefd[1], buffer, sizeof(buffer));
    
    printf("Requested: %lu bytes\n", sizeof(buffer));
    if (count == -1) {
        perror("write");
    } else {
        printf("Actually written: %d bytes\n", count);
        printf("Difference: %lu bytes\n", sizeof(buffer) - count);
    }

    close(pipefd[0]); 
    close(pipefd[1]);
    return 0;
}