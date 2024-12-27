project(09_checksum C CXX)

set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard for all targets.")

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

add_compile_options(-mtune=native -march=native)
add_executable(09_checksum main.cpp)

add_executable(09_test test.cpp)
add_test(NAME "test 09" COMMAND 09_test)

# copy input to build dir
configure_file("input.txt" "input.txt" COPYONLY)
configure_file("test.txt" "test.txt" COPYONLY)
