cmake_minimum_required(VERSION 3.10)
project(morse C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

add_executable(morse
    src/main.c
    src/morse.c
)