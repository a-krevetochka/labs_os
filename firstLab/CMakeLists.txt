cmake_minimum_required(VERSION 3.26)

project(firstLab C)

set(CMAKE_C_STANDARD 11)

include_directories(child)

add_executable(firstLab main.c)

add_subdirectory(child)

target_link_libraries(firstLab child)
