#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fixed_code() {
    void *ptr = NULL;
    int i = 0;

    printf("--- Running Fixed Code ---\n");
    while (i < 2) {
        if (!ptr) {
            ptr = malloc(100);
            if (!ptr) return;
            printf("Iteration %d: Allocated at %p\n", i, ptr);
        }

        strcpy((char*)ptr, "Safe Data"); 
        
        printf("Iteration %d: Freeing %p and nullifying\n", i, ptr);
        free(ptr); 
        ptr = NULL;
        i++;
    }
}

int main() {
    fixed_code();
    return 0;
}