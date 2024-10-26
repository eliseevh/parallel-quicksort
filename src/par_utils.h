#ifndef PAR_UTILS_H
#define PAR_UTILS_H
#include <cstddef>
#include <functional>

int* filter_par(int* arr, size_t size, std::function<bool(int)> predicate, size_t* res_size, size_t block_size);

#endif //PAR_UTILS_H
