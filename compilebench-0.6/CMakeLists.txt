cmake_minimum_required(VERSION 3.10)
project(compile)

set(CMAKE_CXX_STANDARD 11)

# Find the Google Benchmark package
find_package(benchmark REQUIRED)

# Add the first executable
add_executable(compile compile.cpp)

# Link libraries for the first executable
target_link_libraries(compile benchmark::benchmark pthread)

