#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "../quicksort.h"

const size_t ITERS = 500;
const size_t SIZE = 100000;
const size_t BLOCK_SIZE = 1000;

void check(int const* arr, size_t arr_size) {
    for (size_t i = 0; i + 1 < arr_size; ++i) {
        if (arr[i] > arr[i + 1]) {
            exit(1);
        }
    }
}

void main_seq() {
    printf("Start testing seq...\n");
    for (size_t i = 0; i < ITERS; ++i) {
        printf("Run %zd/%zd\n", i + 1, ITERS);
        int* arr = gen_random_arr(SIZE);
        quicksort_seq(arr, SIZE);
        check(arr, SIZE);
    }
}

void main_par() {
    printf("Start testing par...\n");
    for (size_t i = 0; i < ITERS; ++i) {
        printf("Run %zd/%zd\n", i + 1, ITERS);
        int* arr = gen_random_arr(SIZE);
        quicksort_par(arr, SIZE, BLOCK_SIZE);
        check(arr, SIZE);
    }
}

int main() {
    main_par();
    main_seq();
    printf("Passed all\n");
}
