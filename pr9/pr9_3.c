#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    const char *filename = "test_file.txt";
    const char *copy_name = "root_copy.txt";
    FILE *f;

    f = fopen(filename, "w");
    if (f == NULL) { perror("Помилка створення"); return 1; }
    fprintf(f, "Початковий текст від користувача.\n");
    fclose(f);
    printf("1. Файл '%s' створено.\n", filename);

    char cmd[256];
    sprintf(cmd, "sudo cp %s %s", filename, copy_name);
    system(cmd);
    printf("2. Копію '%s' створено від імені root.\n", copy_name);

    printf("3. Спроба змінити файл '%s'...\n", copy_name);
    f = fopen(copy_name, "a");
    if (f == NULL) {
        printf("   РЕЗУЛЬТАТ: Помилка доступу (Permission denied). Неможливо змінити.\n");
    } else {
        fprintf(f, "Спроба запису.\n");
        fclose(f);
    }

    printf("4. Спроба видалити файл '%s' за допомогою rm...\n", copy_name);
    int res = system("rm root_copy.txt");
    if (res == 0) {
        printf("   РЕЗУЛЬТАТ: Файл видалено успішно.\n");
    } else {
        printf("   РЕЗУЛЬТАТ: Не вдалося видалити файл.\n");
    }

    unlink(filename);
    return 0;
}