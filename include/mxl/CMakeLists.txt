cmake_minimum_required(VERSION 3.1)

project("MXL Matrix Library" VERSION 1.0)

add_library(MXL SHARED mxl.hpp)
set_target_properties(MXL PROPERTIES LINKER_LANGUAGE CXX)

install(TARGETS MXL DESTINATION lib)
install(FILES mxl.hpp DESTINATION include)
# add_test(NAME MyTest COMMAND Test)

# add_test(NAME First C)
# add_executable(test1 src/test/test.cpp)
