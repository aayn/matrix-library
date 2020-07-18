cmake_minimum_required(VERSION 3.1)

project("MXL Demo")

set(CMAKE_CXX_STANDARD 11)

add_executable(Demo src/demo.cpp)

include_directories(include include/mxl)

add_executable(Test test/test.cpp)

target_compile_options(Test PUBLIC -g)

enable_testing()

