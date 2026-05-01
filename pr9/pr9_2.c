#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Спроба отримати доступ до /etc/shadow через sudo...\n");

    int status = system("sudo cat /etc/shadow");

    if (status == -1) {
        perror("Помилка виконання system");
        return 1;
    }

    return 0;
}