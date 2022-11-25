cmake_minimum_required(VERSION 3.23)
project(puzzle)

set(CMAKE_CXX_STANDARD 23)

add_executable(puzzle main.cpp bmp.h unicorn.h part.cpp part.h)
