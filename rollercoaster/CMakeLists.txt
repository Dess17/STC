cmake_minimum_required(VERSION 3.14)
project(rollercoaster CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(rollercoaster_lib hills.cpp)
add_executable(rollercoaster main.cpp)
target_link_libraries(rollercoaster rollercoaster_lib)

include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(rollercoaster_test test.cpp)
target_link_libraries(rollercoaster_test rollercoaster_lib GTest::gtest_main)
target_compile_definitions(rollercoaster_test PRIVATE DATA_FILE="${CMAKE_CURRENT_SOURCE_DIR}/data/detector_src_32f.bin")
include(GoogleTest)
gtest_discover_tests(rollercoaster_test)
