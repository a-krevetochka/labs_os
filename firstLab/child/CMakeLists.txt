cmake_minimum_required(VERSION 3.26)

project(child C)

set(CMAKE_C_STANDARD 11)

add_executable(cp child_process.c
        child_process.h)

add_library(child STATIC child_process.c
    child_process.h)

