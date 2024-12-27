project(03_mul C CXX)

set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard for all targets.")

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

add_compile_options(-mtune=native -march=native)
add_executable(03_mul main.cpp)

add_executable(03_test test.cpp)
add_test(NAME "test 03" COMMAND 03_test)

# copy input to build dir
configure_file("input.txt" "input.txt" COPYONLY)
configure_file("test.txt" "test.txt" COPYONLY)
