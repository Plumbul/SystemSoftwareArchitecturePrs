#include "modules.hpp"

void sub_func(int *val) {
    for (int i = 0; i < 1000000; i++) {
        *val -= 1;
    }
}
