#include <cstddef>
#include <cstdlib>
#include <cilk/cilk.h>

void swap(int* p1, int* p2) {
    int tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

void quicksort_seq_impl(int* arr, size_t begin, size_t end) {
    size_t n = end - begin;
    if (n <= 1) {
        return;
    }
    size_t pivot_idx = rand() % n;
    int pivot = arr[begin + pivot_idx];
    size_t less_end = begin;
    size_t greater_begin = less_end;
    for (size_t i = begin; i < end; ++i) {
        if (arr[i] < pivot) {
            swap(arr + i, arr + (less_end++));
            greater_begin++;
        } else if (arr[i] == pivot) {
            swap(arr + i, arr + (greater_begin++));
        }
    }
    quicksort_seq_impl(arr, begin, less_end);
    quicksort_seq_impl(arr, greater_begin, end);
}
void quicksort_seq(int* arr, size_t size) {
    quicksort_seq_impl(arr, 0, size);
}

void quicksort_par_impl(int* arr, size_t begin, size_t end, size_t block_size) {
    size_t n = end - begin;
    if (n <= block_size) {
        quicksort_seq_impl(arr, begin, end);
        return;
    }
    size_t pivot_idx = rand() % n;
    int pivot = arr[begin + pivot_idx];

    // TODO: parallel using filter
    size_t less_end = begin;
    size_t greater_begin = less_end;
    for (size_t i = begin; i < end; ++i) {
        if (arr[i] < pivot) {
            swap(arr + i, arr + (less_end++));
            greater_begin++;
        } else if (arr[i] == pivot) {
            swap(arr + i, arr + (greater_begin++));
        }
    }

    cilk_spawn quicksort_par_impl(arr, begin, less_end, block_size);
    quicksort_par_impl(arr, greater_begin, end, block_size);
    cilk_sync;
}

void quicksort_par(int* arr, size_t size, size_t block_size) {
    quicksort_par_impl(arr, 0, size, block_size);
}
