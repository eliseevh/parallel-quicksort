#include <stdlib.h>
#include "utils.h"

int* gen_random_arr(size_t size) {
    int* res = (int*) malloc(size * sizeof(int));
    for (size_t i = 0; i < size; ++i) {
        res[i] = rand();
    }
    return res;
}
