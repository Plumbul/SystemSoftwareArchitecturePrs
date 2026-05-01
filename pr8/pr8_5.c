#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    char buffer[100];
    int nbytes_requested = 100;

    printf("=== Експеримент 1: Термінал (stdin) ===\n");
    printf("Введіть коротке слово і натисніть Enter (запитуємо %d байтів):\n", nbytes_requested);
    fflush(stdout);
    
    ssize_t n1 = read(0, buffer, nbytes_requested);
    printf("Результат: Запитано: %d, Отримано: %zd\n\n", nbytes_requested, n1);

    printf("=== Експеримент 2: Кінець файлу (EOF) ===\n");
    int fd = open("test_eof.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    write(fd, "Hello!", 6);
    lseek(fd, 0, SEEK_SET);

    ssize_t n2 = read(fd, buffer, nbytes_requested);
    printf("Файл містить 6 байтів. Запитано: %d\n", nbytes_requested);
    printf("Результат: Отримано: %zd\n", n2);

    close(fd);
    unlink("test_eof.txt");
    return 0;
}