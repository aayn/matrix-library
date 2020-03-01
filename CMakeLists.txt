cmake_minimum_required(VERSION 3.1)

project("MXL Demo")

set(CMAKE_CXX_STANDARD 11)

# add_subdirectory(mxl)

add_executable(demo src/demo.cpp)

include_directories(include include/mxl)

add_executable(Test test/test.cpp)
# target_link_libraries(demo MXL)



# add_subdirectory(test)

# include_directories(include)

# add_library(MXL STATIC include/mxl.hpp)
# set_target_properties(MXL PROPERTIES LINKER_LANGUAGE CXX)


enable_testing()
add_test(NAME MyTest COMMAND Test)

# add_test(NAME First C)
# add_executable(test1 src/test/test.cpp)
