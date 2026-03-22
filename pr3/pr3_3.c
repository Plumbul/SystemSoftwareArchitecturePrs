#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 4096

void handle_sigxfsz(int sig) {
    const char *msg = "\n[ERROR] SIGXFSZ: File size limit exceeded during copying!\n";
    write(STDOUT_FILENO, msg, strlen(msg));
    _exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    int src_fd, dest_fd;
    ssize_t bytes_read, bytes_written;
    char buffer[BUFFER_SIZE];

    if (argc != 3) {
        fprintf(stderr, "Program need two arguments\n");
        return 1;
    }

    signal(SIGXFSZ, handle_sigxfsz);

    src_fd = open(argv[1], O_RDONLY);
    if (src_fd == -1) {
        fprintf(stderr, "Cannot open file %s for reading\n", argv[1]);
        return 1;
    }

    dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        fprintf(stderr, "Cannot open file %s for writing\n", argv[2]);
        close(src_fd);
        return 1;
    }

    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(dest_fd, buffer, bytes_read);
        
        if (bytes_written == -1) {
            if (errno == EFBIG) {
                fprintf(stderr, "\nError: File too large (limit reached)\n");
            } else {
                perror("\nWrite error");
            }
            break;
        }
    }

    if (bytes_read == -1) {
        perror("Read error");
    } else if (bytes_written != -1) {
        printf("File copied successfully from %s to %s\n", argv[1], argv[2]);
    }

    close(src_fd);
    close(dest_fd);
    return 0;
}