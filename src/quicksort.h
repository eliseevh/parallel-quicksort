#ifndef QUICKSORT_H
#define QUICKSORT_H

void quicksort_seq(int* arr, size_t size);

void quicksort_par(int* arr, size_t size, size_t block_size);

#endif //QUICKSORT_H
