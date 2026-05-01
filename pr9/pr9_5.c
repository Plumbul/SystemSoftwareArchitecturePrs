#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void check_access(const char *filename) {
    printf("  Доступ на ЧИТАННЯ: %s\n", access(filename, R_OK) == 0 ? "ТАК" : "НІ");
    printf("  Доступ на ЗАПИС:   %s\n", access(filename, W_OK) == 0 ? "ТАК" : "НІ");
}

void run_cmd(const char *cmd) {
    printf("\nВиконується: %s\n", cmd);
    system(cmd);
}

int main() {
    const char *file = "test_perms.txt";

    FILE *f = fopen(file, "w");
    if (!f) { perror("Помилка"); return 1; }
    fprintf(f, "Конфіденційні дані\n");
    fclose(f);
    printf("1. Файл створено. Власник — ви.\n");
    check_access(file);

    char cmd[256];
    sprintf(cmd, "sudo chown root:root %s", file);
    run_cmd(cmd);

    run_cmd("sudo chmod 644 test_perms.txt");
    printf("Результат для 644 (Ви — 'others'):\n");
    check_access(file);

    run_cmd("sudo chmod 600 test_perms.txt");
    printf("Результат для 600 (Ви — 'others'):\n");
    check_access(file);

    run_cmd("sudo chmod 666 test_perms.txt");
    printf("Результат для 666 (Ви — 'others'):\n");
    check_access(file);

    system("sudo rm test_perms.txt");
    return 0;
}