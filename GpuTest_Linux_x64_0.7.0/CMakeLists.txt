cmake_minimum_required(VERSION 3.10)
project(furmark_bench)

set(CMAKE_CXX_STANDARD 11)

# Find the Google Benchmark package
find_package(benchmark REQUIRED)

# Add the first executable
add_executable(furmark_bench furmark_bench.cpp)

# Link libraries for the first executable
target_link_libraries(furmark_bench benchmark::benchmark pthread)

