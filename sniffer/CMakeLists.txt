cmake_minimum_required(VERSION 3.14)
project(sniffer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(sniffer_lib sniffer.cpp)
add_executable(sniffer main.cpp)
target_link_libraries(sniffer sniffer_lib)

include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(sniffer_test test.cpp)
target_link_libraries(sniffer_test sniffer_lib GTest::gtest_main)
target_compile_definitions(sniffer_test PRIVATE LOG_FILE="${CMAKE_CURRENT_SOURCE_DIR}/data/frames_parser.log")
include(GoogleTest)
gtest_discover_tests(sniffer_test)
