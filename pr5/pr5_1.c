#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("--- Tcache Double Free Experiment ---\n");

    void *p1 = malloc(32);
    printf("Allocated block at: %p\n", p1);

    printf("First free(p1)...\n");
    free(p1);

    printf("Second free(p1) - Double Free!\n");
    free(p1); 

    printf("This line will not be reached if protection works.\n");
    return 0;
}