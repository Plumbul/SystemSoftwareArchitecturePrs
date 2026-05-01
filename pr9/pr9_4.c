#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("--- Перевірка поточного облікового запису ---\n\n");

    printf("Результат команди 'whoami': ");
    fflush(stdout);
    system("whoami");

    printf("\n--- Детальна інформація про ідентифікатори ---\n");

    system("id");

    printf("\n--------------------------------------------\n");
    return 0;
}