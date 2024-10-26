#include <cstddef>
#include <cstdio>
#include "quicksort.h"

void main_seq();
void main_par();

void print_arr(int* arr, size_t size) {
    printf("[");
    for (size_t i = 0; i < size; ++i) {
        if (i != 0) {
            printf(", ");
        }
        printf("%d", arr[i]);
    }
    printf("]");
}
int main(void) {
    printf("Seq:\n");
    main_seq();
    printf("Par:\n");
    main_par();
}

void main_seq() {
    int arr[4] = {0, -4, 10, 2};
    int size = sizeof(arr) / sizeof(int);
    printf("Initial: ");
    print_arr(arr, size);
    quicksort_seq(arr, size);
    printf("\nAfter sort: ");
    print_arr(arr, size);
    printf("\n");
}
void main_par() {
    int arr[4] = {0, -4, 10, 2};
    int size = sizeof(arr) / sizeof(int);
    printf("Initial: ");
    print_arr(arr, size);
    quicksort_par(arr, size, 1);
    printf("\nAfter sort: ");
    print_arr(arr, size);
    printf("\n");
}
