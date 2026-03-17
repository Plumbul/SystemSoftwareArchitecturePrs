#include <omp.h>
#include "modules.h"

void add_func(int *val) {
    int local_sum = 0;

    #pragma omp parallel for reduction(+:local_sum)
    for (int i = 0; i < 1000000; i++) {
        local_sum++;
    }

    #pragma omp atomic
    *val += local_sum;
}
