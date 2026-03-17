#include <stdio.h>
#include <omp.h>
#include "modules.h"

int main() {
    int initial_data = 100;

    printf("Initial value: %d\n", initial_data);

   #pragma omp parallel sections
    {
        #pragma omp section
        {
            add_func(&initial_data);
        }

        #pragma omp section
        {
            sub_func(&initial_data);
        }
    }

    printf("Result (OpenMP): %d\n", initial_data);
    return 0;
}
