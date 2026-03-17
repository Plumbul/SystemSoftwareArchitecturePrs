#include <stdio.h>
#include "modules.h"

int main() {
    int initial_data = 100;
    printf("Initial value: %d\n", initial_data);

    add_func(&initial_data);
    sub_func(&initial_data);

    printf("Result: %d\n", initial_data);
    return 0;
}
