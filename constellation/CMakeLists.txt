cmake_minimum_required(VERSION 3.14)
project(constellation CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(constellation main.cpp plot.cpp plot.h reader.cpp reader.h)
target_link_libraries(constellation Qt5::Widgets)
target_compile_definitions(constellation PRIVATE DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
