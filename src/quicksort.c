#include <stddef.h>
#include <stdlib.h>
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
    for (size_t i = begin; i < end; ++i) {
        if (arr[i] < pivot) {
            swap(arr + i, arr + (less_end++));
        }
    }
    size_t greater_begin = less_end;
    for (size_t i = less_end; i < end; ++i) {
        if (arr[i] == pivot) {
            swap(arr + i, arr + (greater_begin++));
        }
    }
    quicksort_seq_impl(arr, begin, less_end);
    quicksort_seq_impl(arr, greater_begin, end);
}
void quicksort_seq(int* arr, size_t size) {
    quicksort_seq_impl(arr, 0, size);
}
void quicksort_par_impl(int* arr, size_t begin, size_t end, size_t block_size);
size_t qpivl(int* arr, size_t begin, size_t end, int pivot, size_t block_size) {
    int* l = (int*) malloc((end - begin) * sizeof(int));
    size_t cnt = 0;
    for (size_t i = begin; i < end; ++i) {
        if (arr[i] < pivot) {
            l[cnt++] = arr[i];
        }
    }
    quicksort_par_impl(l, 0, cnt, block_size);
    for (size_t i = 0; i < cnt; ++i) {
        arr[i + begin] = l[i];
    }
    free(l);
    return cnt;
}

size_t qpivg(int* arr, size_t begin, size_t end, int pivot, size_t block_size) {
    int* g = (int*)malloc((end - begin) * sizeof(int));
    size_t cnt = 0;
    for (size_t i = begin; i < end; ++i) {
        if (arr[i] > pivot) {
            g[cnt++] = arr[i];
        }
    }
    quicksort_par_impl(g, 0, cnt, block_size);
    for (size_t i = 0; i < cnt; ++i) {
        arr[end - cnt + i] = g[i];
    }
    free(g);
    return cnt;
}

void quicksort_par_impl(int* arr, size_t begin, size_t end, size_t block_size) {
    size_t n = end - begin;
    if (n <= block_size) {
        quicksort_seq_impl(arr, begin, end);
        return;
    }
    size_t pivot_idx = rand() % n;
    int pivot = arr[begin + pivot_idx];

    size_t less = cilk_spawn qpivl(arr, begin, end, pivot, block_size);

    size_t greater = qpivg(arr, begin, end, pivot, block_size);
    cilk_sync;

    for (size_t i = begin + less; i < end - greater; ++i) {
        arr[i] = pivot;
    }
}

void quicksort_par(int* arr, size_t size, size_t block_size) {
    quicksort_par_impl(arr, 0, size, block_size);
}
