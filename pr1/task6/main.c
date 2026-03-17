#include <stdio.h>
#include <string.h>
#include <aio.h>
#include <unistd.h>
#include <pthread.h>
#include "modules.h"

void async_print(const char *msg) {
    static struct aiocb cb;
    memset(&cb, 0, sizeof(struct aiocb));
    cb.aio_fildes = STDOUT_FILENO;
    cb.aio_buf = (void *)msg;
    cb.aio_nbytes = strlen(msg);

    if (aio_write(&cb) == -1) {
        perror("aio_write failed");
    }
}

int main() {
    int initial_data = 100;
    char msg[64];
    sprintf(msg, "Initial value: %d\n", initial_data);
    async_print(msg);

    pthread_t t1, t2;
    pthread_create(&t1, NULL, add_func, &initial_data);
    pthread_create(&t2, NULL, sub_func, &initial_data);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Result: %d\n", initial_data);
    return 0;
}
