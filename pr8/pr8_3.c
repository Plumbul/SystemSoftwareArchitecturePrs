#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

long long compare_count = 0;

int compare_int(const void *a, const void *b) {
    compare_count++;
    return (*(int*)a - *(int*)b);
}

void run_experiment(int n, const char *test_name, int type) {
    int *arr = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        if (type == 0) arr[i] = i;
        else if (type == 1) arr[i] = n - i;
        else if (type == 2) arr[i] = 42;
        else arr[i] = rand() % n;
    }

    compare_count = 0;
    qsort(arr, n, sizeof(int), compare_int);

    printf("%-20s | Розмір: %d | Порівнянь: %lld\n", test_name, n, compare_count);
    free(arr);
}

void verify_sort(int n) {
    int *arr = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) arr[i] = rand() % 1000;

    qsort(arr, n, sizeof(int), compare_int);

    int sorted = 1;
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            sorted = 0;
            break;
        }
    }
    printf("Тест на правильність: %s\n", sorted ? "ПРОЙДЕНО" : "ПОМИЛКА");
    free(arr);
}

int main() {
    srand(time(NULL));
    int sizes[] = {1000, 5000, 10000};

    printf("=== Експеримент: Пошук найгірших даних для qsort ===\n");
    for (int i = 0; i < 3; i++) {
        run_experiment(sizes[i], "Sorted", 0);
        run_experiment(sizes[i], "Reverse", 1);
        run_experiment(sizes[i], "All Identical", 2);
        run_experiment(sizes[i], "Random", 3);
        printf("--------------------------------------------------\n");
    }

    printf("\n=== Перевірка правильності qsort ===\n");
    verify_sort(100);
    
    return 0;
}