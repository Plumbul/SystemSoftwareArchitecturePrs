#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>

void infinite_recursion(int depth) {
    char buffer[1024]; 
    if (depth % 100 == 0) {
        printf("Current stack depth: %d (approx %d KB used)\n", depth, depth);
    }
    infinite_recursion(depth + 1);
}

int main() {
    struct rlimit limit;

    limit.rlim_cur = 64 * 1024; 
    limit.rlim_max = 64 * 1024;

    if (setrlimit(RLIMIT_STACK, &limit) != 0) {
        perror("setrlimit error");
        return 1;
    }

    printf("Stack limit set to %ld bytes. Starting recursion...\n", limit.rlim_cur);

    infinite_recursion(1);

    return 0;
}