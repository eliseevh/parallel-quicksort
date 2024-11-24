cmake_minimum_required(VERSION 3.10)
project(quicksort C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopencilk")

add_library(quicksort_lib src/quicksort.c)

add_executable(test_correct src/test/utils.c src/test/test_correct.c)

add_executable(test_time src/test/utils.c src/test/test_time.c)

target_link_libraries(test_correct quicksort_lib)

target_link_libraries(test_time quicksort_lib)
