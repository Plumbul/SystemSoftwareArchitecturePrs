#include <stdio.h>
#include <pthread.h>
#include "modules.h"

int main() {
    pthread_t thread1, thread2;
    int initial_data = 100;

    printf("Initial value: %d\n", initial_data);

    pthread_create(&thread1, NULL, add_func, &initial_data);
    pthread_create(&thread2, NULL, sub_func, &initial_data);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Result: %d\n", initial_data);
    return 0;
}
