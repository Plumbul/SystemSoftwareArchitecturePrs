#include <stdio.h>
#include <stdlib.h>

int main() {
    size_t initial_size = 1024;
    void *ptr = malloc(initial_size);
    
    if (!ptr) return 1;
    printf("Initial pointer: %p\n", ptr);

    size_t extreme_size = (size_t)-1 / 2; 
    printf("Attempting to realloc to a huge size...\n");

    void *tmp = realloc(ptr, extreme_size);

    if (tmp == NULL) {
        printf("realloc(3) failed: returned NULL\n");
        printf("Status: Original pointer %p is still valid and NOT leaked.\n", ptr);
        free(ptr);
    } else {
        ptr = tmp;
        printf("realloc(3) succeeded at %p\n", ptr);
        free(ptr);
    }

    return 0;
}