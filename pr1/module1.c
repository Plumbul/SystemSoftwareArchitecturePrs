#include <stdio.h>
#include "modules.h"

void* add_func(void* arg) {
    int *val = (int*)arg;
    for (int i = 0; i < 1000000; i++) *val +=1;
    return NULL;
}
