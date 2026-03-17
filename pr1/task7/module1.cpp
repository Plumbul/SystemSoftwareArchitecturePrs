#include "modules.hpp"

void add_func(int *val) {
    for (int i = 0; i < 1000000; i++) {
        *val += 1;
    }
}
