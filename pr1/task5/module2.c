#include "modules.h"
#include <omp.h>

void sub_func(int *val) {
    int local_sub = 0;

    #pragma omp parallel for reduction(+:local_diff)
    for (int i = 0; i < 1000000; i++) {
        local_sub++; 
    }

    #pragma omp atomic
    *val -= local_sub;
}
