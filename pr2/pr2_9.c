#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void test_function() {}
int initialized_global = 42;
int uninitialized_global;

void expand_stack(int depth, uintptr_t main_stack_addr) {
    double large_array[1000]; 
    large_array[0] = (double)depth; 
    if (depth > 0) {
        expand_stack(depth - 1, main_stack_addr);
    } else {
        uintptr_t current_stack = (uintptr_t)&large_array[0];
        printf("Stack top (after expansion):  %p\n", (void*)current_stack);
        printf("Stack grew down by:           %ld bytes\n", (long)(main_stack_addr - current_stack));
    }
}

int main() {
    int *heap_var = (int*)malloc(sizeof(int));
    int main_stack_var;
    uintptr_t main_addr = (uintptr_t)&main_stack_var;

    printf("--- Segments addresses ---\n");
    printf("Text segment (code):         %p\n", (void*)test_function);
    printf("Data segment (init global):  %p\n", (void*)&initialized_global);
    printf("BSS segment (uninit):        %p\n", (void*)&uninitialized_global);
    printf("Heap (dynamic):              %p\n", (void*)heap_var);
    printf("Stack top (in main):         %p\n", (void*)main_addr);

    printf("\n--- Stack expansion (10 levels deep) ---\n");
    expand_stack(10, main_addr);

    free(heap_var);
    return 0;
}