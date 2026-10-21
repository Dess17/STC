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
