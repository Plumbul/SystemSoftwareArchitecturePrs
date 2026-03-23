#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main() {
    size_t step = 1024ULL * 1024 * 1024;
    size_t current_request = step;
    void *ptr = NULL;

    printf("--- Malloc Limits Testing ---\n");
    printf("Size of size_t: %zu bytes\n", sizeof(size_t));

    while (1) {
        ptr = malloc(current_request);

        if (ptr != NULL) {
            printf("[SUCCESS] Allocated: %zu GB\n", current_request / (1024 * 1024 * 1024));
            free(ptr);
            
            if (current_request > (SIZE_MAX - step)) {
                printf("Reached SIZE_MAX limit!\n");
                break;
            }
            current_request += step;
        } else {
            printf("[FAILED] Could not allocate: %zu GB\n", current_request / (1024 * 1024 * 1024));
            
            if (step > 1024 * 1024) { 
                printf("Refining search with smaller steps (1 MB)...\n");
                current_request -= step;
                step = 1024 * 1024;  
                current_request += step;
            } else {
                printf("Final limit reached at approximately: %zu MB\n", current_request / (1024 * 1024));
                break;
            }
        }
    }

    return 0;
}