project(11_task C CXX)

set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard for all targets.")

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

include_directories(${CMAKE_SOURCE_DIR}/common)

add_compile_options(-mtune=native -march=native)
add_executable(11_task main.cpp)

add_executable(11_test test.cpp)
add_test(NAME "test 11" COMMAND 11_test)

# copy input to build dir
configure_file("input.txt" "input.txt" COPYONLY)
configure_file("test.txt" "test.txt" COPYONLY)
