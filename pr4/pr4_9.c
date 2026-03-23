#include <stdlib.h>
#include <stdio.h>

struct sbar {
    int id;
    char data[60];
};

int main() {
    struct sbar *ptr, *newptr;

    printf("Step 1: Allocating memory for 1000 elements...\n");
    ptr = calloc(1000, sizeof(struct sbar));
    if (!ptr) {
        perror("calloc failed");
        return 1;
    }
    printf("Initial pointer address: %p\n", (void*)ptr);

    printf("Step 2: Resizing to 500 elements using reallocarray...\n");
    newptr = reallocarray(ptr, 500, sizeof(struct sbar));

    if (newptr == NULL) {
        perror("reallocarray failed");
        free(ptr);
        return 1;
    }

    ptr = newptr;
    printf("New pointer address:     %p\n", (void*)ptr);
    printf("Memory successfully resized and verified.\n");
    
    free(ptr);
    printf("Memory freed. Process finished.\n");
    return 0;
}