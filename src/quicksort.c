#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
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

void scan_sum_par(int* arr, size_t begin, size_t end, size_t block_size) {
    if (end - begin <= block_size) {
        for (size_t i = begin; i + 1 < end; ++i) {
            arr[i + 1] += arr[i];
        }
        return;
    }
    size_t med = (end + begin) / 2;
    cilk_spawn scan_sum_par(arr, begin, med, block_size);
    scan_sum_par(arr, med, end, block_size);
    cilk_sync;
    int last_val = arr[med - 1];
    cilk_for (size_t i = med; i < end; ++i) {
        arr[i] += last_val;
    }
}

size_t filter_par(int const* arr, size_t begin, size_t end, int** res, size_t block_size, int* pred) {
    scan_sum_par(pred, 0, end - begin, block_size);
    size_t cnt = pred[end - begin - 1];
    *res = (int*) malloc(cnt * sizeof(int));
    cilk_for (size_t i = 0; i < end - begin; ++i) {
        bool filtered;
        if (i == 0) {
            filtered = pred[i] == 1;
        } else {
            filtered = pred[i] > pred[i - 1];
        }
        if (filtered) {
            (*res)[pred[i] - 1] = arr[i + begin];
        }
    }
    return cnt;
}

size_t qpivl(int const* arr, size_t begin, size_t end, int pivot, int** res, size_t block_size) {
    int* l = (int*) malloc((end - begin) * sizeof(int));
    cilk_for (size_t i = begin; i < end; ++i) {
        if (arr[i] < pivot) {
            l[i - begin] = 1;
        } else {
            l[i - begin] = 0;
        }
    }
    size_t cnt = filter_par(arr, begin, end, res, block_size, l);
    free(l);
    quicksort_par_impl(*res, 0, cnt, block_size);
    return cnt;
}

size_t qpivg(int const* arr, size_t begin, size_t end, int pivot, int** res, size_t block_size) {
    int* g = (int*)malloc((end - begin) * sizeof(int));
    cilk_for (size_t i = begin; i < end; ++i) {
        if (arr[i] > pivot) {
            g[i - begin] = 1;
        } else {
            g[i - begin] = 0;
        }
    }
    size_t cnt = filter_par(arr, begin, end, res, block_size, g);
    free(g);
    quicksort_par_impl(*res, 0, cnt, block_size);
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
    int* less_part;
    int* greater_part;
    size_t less = cilk_spawn qpivl(arr, begin, end, pivot, &less_part, block_size);

    size_t greater = qpivg(arr, begin, end, pivot, &greater_part, block_size);
    cilk_sync;
    cilk_for (size_t i = begin; i < begin + less; ++i) {
        arr[i] = less_part[i - begin];
    }
    free(less_part);
    cilk_for (size_t i = end - greater; i < end; ++i) {
        arr[i] = greater_part[i - end + greater];
    }
    free(greater_part);
    cilk_for (size_t i = begin + less; i < end - greater; ++i) {
        arr[i] = pivot;
    }
}

void quicksort_par(int* arr, size_t size, size_t block_size) {
    quicksort_par_impl(arr, 0, size, block_size);
}
