#include <stdio.h>
#include <stdlib.h>

void test_function() {}

int initialized_global = 42;

int uninitialized_global;

void expand_stack(int depth) {
    double large_array[1000]; 
    
    if (depth > 0) {
        expand_stack(depth - 1);
    } else {
        printf("Stack top (after expansion): %p\n", (void*)&large_array[0]);
    }
}

int main() {
    int *heap_var = (int*)malloc(sizeof(int));

    printf("--- Segments adresses ---\n");
    printf("Text segment (code):        %p\n", (void*)test_function);
    printf("Data segment (init global): %p\n", (void*)&initialized_global);
    printf("BSS segment (uninit):       %p\n", (void*)&uninitialized_global);
    printf("Heap (dynamic):             %p\n", (void*)heap_var);
    
    int main_stack_var;
    printf("Stack top (in main):        %p\n", (void*)&main_stack_var);

    printf("\n--- Stack expansion ---\n");
    expand_stack(5);

    free(heap_var);
    return 0;
}