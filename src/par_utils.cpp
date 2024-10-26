#include <cilk/cilk.h>
#include <cstdlib>
#include "par_utils.h"

void parallel_for(size_t begin, size_t end, std::function<void(size_t)> const& f, size_t block_size) {
    if (end - begin <= block_size) {
        for (size_t i = begin; i < end; ++i) {
            f(i);
        }
        return;
    }
    size_t med = (begin + end) / 2;
    cilk_spawn parallel_for(begin, med, f, block_size);
    parallel_for(med, end, f, block_size);
    cilk_sync;
}

void map_inplace_par(int* arr, size_t begin, size_t end, std::function<int(int)> const& f, size_t block_size) {
    auto iter = [&](size_t i) {
        arr[i] = f(arr[i]);
    };
    parallel_for(begin, end, iter, block_size);
}

void scan_inplace_par(int* arr, size_t begin, size_t end, size_t block_size) {
    if (end - begin <= block_size) {
        for (size_t i = begin; i + 1 < end; ++i) {
            arr[i + 1] += arr[i];
        }
        return;
    }
    size_t med = (end - begin) / 2;
    cilk_spawn scan_inplace_par(arr, begin, med, block_size);
    scan_inplace_par(arr, med, end, block_size);
    cilk_sync;
    int last_val = arr[med - 1];
    auto add_last_val = [&](int v) {
        return v + last_val;
    };
    map_inplace_par(arr, med, end, add_last_val, block_size);
}

int* filter_par(int const* arr, size_t size, std::function<bool(int)> predicate, size_t* res_size, size_t block_size) {
    // Create copy, to not damage original array
    int* arr_cpy = (int*) malloc(size);
    auto cpy_iter = [&](size_t i) {
        arr_cpy[i] = arr[i];
    };
    parallel_for(0, size, cpy_iter, block_size);
    // Actual filter
    auto mapping_from_predicate = [&](int v) {
        return predicate(v) ? 1 : 0;
    };
    map_inplace_par(arr_cpy, 0, size, mapping_from_predicate, block_size);
    scan_inplace_par(arr_cpy, 0, size, block_size);
    *res_size = arr_cpy[size - 1];
    int* result = (int*) malloc(*res_size);
    auto filter_move_iter = [&](size_t i) {
        bool filtered;
        if (i == 0) {
            filtered = arr_cpy[i] == 1;
        } else {
            filtered = arr_cpy[i] > arr_cpy[i - 1];
        }
        if (filtered) {
            result[arr_cpy[i] - 1] = arr[i];
        }
    };
    parallel_for(0, size, filter_move_iter, block_size);
    free(arr_cpy);
    return result;
}
