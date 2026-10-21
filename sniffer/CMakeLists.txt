cmake_minimum_required(VERSION 3.14)
project(sniffer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(sniffer_lib sniffer.cpp)
add_executable(sniffer main.cpp)
target_link_libraries(sniffer sniffer_lib)
