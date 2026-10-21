cmake_minimum_required(VERSION 3.14)
project(counters CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_library(counters_lib counters.cpp)
target_link_libraries(counters_lib Threads::Threads)
add_executable(counters main.cpp)
target_link_libraries(counters counters_lib)

if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/data_raw_32_rand_ch_offs_break)
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${CMAKE_CURRENT_SOURCE_DIR}/data/data_raw_32_ok.zip
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${CMAKE_CURRENT_SOURCE_DIR}/data/data_raw_32_error.zip
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(counters_test test.cpp)
target_link_libraries(counters_test counters_lib GTest::gtest_main)
target_compile_definitions(counters_test PRIVATE
    OK_DIR="${CMAKE_CURRENT_BINARY_DIR}/data_raw_32_rand_ch_offs"
    ERROR_DIR="${CMAKE_CURRENT_BINARY_DIR}/data_raw_32_rand_ch_offs_break")
include(GoogleTest)
gtest_discover_tests(counters_test)
