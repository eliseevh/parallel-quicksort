set -xe

mkdir -p cmake-build-release && cd cmake-build-release
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang ..

cmake --build . --target test_time
cmake --build . --target test_correct

./test_correct

./test_time 2> ../bench_results.txt
