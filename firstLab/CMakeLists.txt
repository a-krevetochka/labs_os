cmake_minimum_required(VERSION 3.13)

project(firstLab)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-g)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(ZeroMQ REQUIRED)

include_directories(${INCLUDE_DIR})

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(AMPQ ${CMAKE_CURRENT_SOURCE_DIR}/AMPQ-CPP/include)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false ${SOURCE_DIR}/*.cpp)

add_executable(${CMAKE_PROJECT_NAME}_exe main.cpp)

add_executable(child_exe child.cpp)






