cmake_minimum_required(VERSION 3.26)

project(firstLab C)

set(CMAKE_C_STANDARD 11)

add_executable(parent main.c)

add_executable(child child_process.c)