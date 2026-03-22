#include <stdio.h>
#include <unistd.h>

void deep_function(int n) {
    int large_array[100];
    if (n > 0) {
        deep_function(n - 1);
    } else {
        printf("Programm sleep. PID: %d\n", getpid());
        sleep(60);
    }
}

int main() {
    deep_function(5);
    return 0;
}