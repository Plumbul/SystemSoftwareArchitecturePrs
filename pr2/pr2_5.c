#include <stdio.h>

void local_function() {
    int local_array[1000];
    
    int local_init_array[1000] = {1, 2, 3};

    printf("Local arrays\n");

    printf("Adress: %p, %p\n", (void*)local_array, (void*)local_init_array);
}

int main() {
    
    printf("hello world\n");
    local_function();
    return 0;
}