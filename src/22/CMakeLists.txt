project(22_task C CXX)

set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard for all targets.")

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

include_directories(${CMAKE_SOURCE_DIR}/common)

add_compile_options(-mtune=native -march=native)
add_compile_options("$<$<CONFIG:Release>:-O3>")

add_executable(22_task main.cpp)

add_executable(22_test test.cpp)
add_test(NAME "test 22" COMMAND 22_test)
#target_compile_options(22_test PRIVATE  -pg)
#target_link_options(22_test PRIVATE  -pg)

# copy input to build dir
configure_file("input.txt" "input.txt" COPYONLY)
configure_file("test.txt" "test.txt" COPYONLY)
configure_file("test2.txt" "test2.txt" COPYONLY)
