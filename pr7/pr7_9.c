#include <stdio.h>
#include <sys/time.h>

int main() {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (long i = 0; i < 100000000; i++);
    gettimeofday(&end, NULL);
    long ms = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Execution time: %ld ms\n", ms);
    return 0;
}