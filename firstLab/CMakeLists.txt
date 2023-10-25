cmake_minimum_required(VERSION 3.26)

project(firstLab)

set(CMAKE_C_STANDARD 11)

add_executable(parent main.cpp)

add_executable(child child_process.cpp)