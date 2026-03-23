#include <stdio.h>
#include <stdlib.h>

struct sbar {
    int id;
    double values[5];
};

int main() {
    struct sbar *ptr, *newptr;

    printf("--- Step 1: Initializing memory with calloc(1000) ---\n");
    ptr = (struct sbar *)calloc(1000, sizeof(struct sbar));
    
    if (ptr == NULL) {
        perror("Initial allocation failed");
        return 1;
    }
    printf("Address after calloc: %p\n", (void*)ptr);

    printf("\n--- Step 2: Resizing to 500 elements with reallocarray ---\n");
    newptr = (struct sbar *)reallocarray(ptr, 500, sizeof(struct sbar));

    if (newptr == NULL) {
        perror("Reallocarray failed");
        free(ptr);
        return 1;
    }

    ptr = newptr;
    printf("Address after reallocarray: %p\n", (void*)ptr);
    printf("Memory successfully resized to 500 elements.\n");

    free(ptr);
    printf("\nMemory freed. Process finished.\n");
    
    return 0;
}