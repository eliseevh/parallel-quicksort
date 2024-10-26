#ifndef PAR_UTILS_H
#define PAR_UTILS_H
#include <cstddef>
#include <functional>

void parallel_for(size_t begin, size_t end, std::function<void(size_t)> const& f, size_t block_size);

int* filter_par(int const* arr, size_t begin, size_t end, std::function<bool(int)> const& predicate, size_t* res_size,
                size_t
block_size);

#endif //PAR_UTILS_H
