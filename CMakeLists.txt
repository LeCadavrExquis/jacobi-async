cmake_minimum_required(VERSION 3.26)
project(jacobi_async C)

set(CMAKE_C_STANDARD 11)

# Wyszukiwanie i konfiguracja OpenMP
find_package(OpenMP REQUIRED)
if(OpenMP_C_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_C_FLAGS}")
endif()

add_executable(
        jacobi_async
        main.c
        utils.h
        utils.c
        alg.c
        alg.h
)

target_link_libraries(jacobi_async m)
if(OpenMP_C_FOUND)
    target_link_libraries(jacobi_async OpenMP::OpenMP_C)
endif()
