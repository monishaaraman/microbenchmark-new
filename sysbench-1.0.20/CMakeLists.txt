cmake_minimum_required(VERSION 3.10)
project(sys_bench)

set(CMAKE_CXX_STANDARD 11)

# Find the Google Benchmark package
find_package(benchmark REQUIRED)

# Add the first executable
add_executable(sys_bench sys_bench.cpp)

# Link libraries for the first executable
target_link_libraries(sys_bench benchmark::benchmark pthread)

