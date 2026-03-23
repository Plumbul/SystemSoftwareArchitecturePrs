#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Requesting 0 bytes via malloc(0)...\n");
    
    void *ptr = malloc(0);

    if (ptr == NULL) {
        printf("Result: malloc(0) returned NULL\n");
    } else {
        printf("Result: malloc(0) returned a non-NULL pointer: %p\n", ptr);
        printf("Freeing the pointer...\n");
        free(ptr);
        printf("Success!\n");
    }

    return 0;
}