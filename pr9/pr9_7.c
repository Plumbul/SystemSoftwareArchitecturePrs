#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

void test_fs(const char *path) {
    char filename[256];
    sprintf(filename, "%s/test_file_000", path);

    printf("\n--- Тестування в каталозі: %s ---\n", path);

    if (creat(filename, 0000) == -1) {
        printf("Помилка створення: %s (Можливо, ФС не підтримує такі права)\n", strerror(errno));
        return;
    }

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        printf("Результат для прав 000: Доступ заборонено (Очікувана поведінка)\n");
    } else {
        printf("Результат для прав 000: ДОСТУП ДОЗВОЛЕНО (Дивна поведінка ФС!)\n");
        close(fd);
    }

    unlink(filename);
}

int main() {
    test_fs("/tmp");
    test_fs(".");
    test_fs("/mnt/nfs"); 
    
    return 0;
}