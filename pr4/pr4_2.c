#include <stdio.h>
#include <stdlib.h>

int main() {
    int xa = 1024 * 1024;
    int xb = 3000;
    
    int num = xa * xb;
    
    printf("xa: %d, xb: %d\n", xa, xb);
    printf("Resulting num (int): %d (Negative due to overflow)\n", num);
    printf("Size of size_t: %zu bytes\n", sizeof(size_t));

    void *ptr = malloc(num);

    if (ptr == NULL) {
        printf("malloc(3) failed: returned NULL\n");
    } else {
        printf("malloc(3) succeeded!\n");
        free(ptr);
    }

    return 0;
}