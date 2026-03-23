#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void broken_code() {
    void *ptr = NULL;
    int i = 0;

    printf("--- Running Broken Code ---\n");
    while (i < 2) {
        if (!ptr) {
            ptr = malloc(100);
            printf("Iteration %d: Allocated at %p\n", i, ptr);
        } else {
            printf("Iteration %d: Using OLD pointer %p (Danger!)\n", i, ptr);
        }

        strcpy((char*)ptr, "Data"); 
        
        printf("Iteration %d: Freeing %p\n", i, ptr);
        free(ptr); 
        i++;
    }
}

int main() {
    broken_code();
    return 0;
}