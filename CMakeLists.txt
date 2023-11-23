cmake_minimum_required(VERSION 3.26)
project(jacobi_async C)

set(CMAKE_C_STANDARD 11)

add_executable(
        jacobi_async main.c
        utils.h
        utils.c
)
