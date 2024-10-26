cmake_minimum_required(VERSION 3.10)
project(quicksort CXX)

set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

add_executable(quicksort src/quicksort.cpp src/main.cpp src/par_utils.cpp)
