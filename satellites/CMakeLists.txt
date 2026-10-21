cmake_minimum_required(VERSION 3.14)
project(satellites CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

find_package(Qt5 REQUIRED COMPONENTS Widgets Network)

add_executable(satellites main.cpp mainwindow.cpp mainwindow.h tle.cpp tle.h)
target_link_libraries(satellites Qt5::Widgets Qt5::Network)
