#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    const char *filename = "fork_test.txt";
    
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Помилка відкриття файлу");
        return 1;
    }

    printf("Файл відкритий. Виконую fork()...\n");

    pid_t pid = fork();

    if (pid < 0) {
        perror("Помилка fork");
        return 1;
    } else if (pid == 0) {
        const char *child_msg = "Це запис ДОЧІРНЬОГО процесу.\n";
        write(fd, child_msg, 50);
        printf("Дочірній процес виконав запис.\n");
        close(fd);
        exit(0);
    } else {
        const char *parent_msg = "Це запис БАТЬКІВСЬКОГО процесу.\n";
        write(fd, parent_msg, 50);
        printf("Батьківський процес виконав запис.\n");
        
        wait(NULL);
        close(fd);
    }

    printf("\nВміст файлу %s:\n", filename);
    system("cat fork_test.txt");

    return 0;
}