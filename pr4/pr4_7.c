#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("--- Testing realloc(NULL, size) ---\n");
    void *p1 = realloc(NULL, 100);
    if (p1 != NULL) {
        printf("realloc(NULL, 100) succeeded. Pointer: %p\n", p1);
    }

    printf("\n--- Testing realloc(ptr, 0) ---\n");
    void *p2 = realloc(p1, 0);
    
    printf("realloc(p1, 0) returned: %p\n", p2);

    if (p2 != NULL) {
        free(p2);
        printf("Pointer p2 freed.\n");
    }

    return 0;
}