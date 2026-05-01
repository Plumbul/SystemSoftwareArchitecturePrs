#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int fd;
    unsigned char buffer[4];
    unsigned char data[] = {4, 5, 2, 2, 3, 3, 7, 9, 1, 5};

    fd = open("test_data.bin", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Помилка відкриття файлу");
        return 1;
    }
    write(fd, data, sizeof(data));

    if (lseek(fd, 3, SEEK_SET) == -1) {
        perror("Помилка lseek");
        return 1;
    }

    ssize_t bytes_read = read(fd, buffer, 4);
    if (bytes_read < 0) {
        perror("Помилка читання");
        return 1;
    }

    printf("Вміст буфера після read: ");
    for (int i = 0; i < bytes_read; i++) {
        printf("%d ", buffer[i]);
    }
    printf("\n");

    close(fd);
    unlink("test_data.bin");
    return 0;
}