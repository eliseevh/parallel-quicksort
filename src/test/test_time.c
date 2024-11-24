#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "utils.h"
#include "../quicksort.h"

const size_t ITERS = 5;
const size_t SIZE = 100000000;
const size_t BLOCK_SIZE = 1000;

time_t cur_time() {
    struct timeval tv = {0};

    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void check(int const* arr, size_t arr_size) {
    for (size_t i = 0; i + 1 < arr_size; ++i) {
        if (arr[i] > arr[i + 1]) {
            exit(1);
        }
    }
}

float main_seq() {
    time_t times[ITERS];
    printf("Start running seq...\n");
    for (size_t i = 0; i < ITERS; ++i) {
        printf("Run %zd/%zd\n", i + 1, ITERS);
        int* arr = gen_random_arr(SIZE);
        time_t start_it = cur_time();
        quicksort_seq(arr, SIZE);
        times[i] = cur_time() - start_it;
        free(arr);
    }
    time_t times_sum = 0;
    printf("Sequential times:");
    for (size_t i = 0; i < ITERS; ++i) {
        times_sum += times[i];
        printf(" %ld", times[i]);
        if (i != ITERS - 1) {
            printf(",");
        } else {
            printf("\n");
        }
    }
    float avg = (float)times_sum / ITERS;
    printf("Sequential average time: %0.2f\n", avg);
    return avg;
}

float main_par() {
    time_t times[ITERS];
    printf("Start running par...\n");
    for (size_t i = 0; i < ITERS; ++i) {
        printf("Run %zd/%zd\n", i + 1, ITERS);
        int* arr = gen_random_arr(SIZE);
        time_t start_it = cur_time();
        quicksort_par(arr, SIZE, BLOCK_SIZE);
        times[i] = cur_time() - start_it;
        free(arr);
    }
    time_t times_sum = 0;
    printf("Parallel times:");
    for (size_t i = 0; i < ITERS; ++i) {
        times_sum += times[i];
        printf(" %ld", times[i]);
        if (i != ITERS - 1) {
            printf(",");
        } else {
            printf("\n");
        }
    }
    float avg = (float)times_sum / ITERS;
    printf("Parallel average time: %0.2f\n", avg);
    return avg;
}

int main() {
    float avg_par = main_par();
    float avg_seq = main_seq();
    printf("Parallel average time is %0.2f times lower than sequential\n", avg_seq / avg_par);
}
