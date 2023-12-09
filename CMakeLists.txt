cmake_minimum_required(VERSION 3.26)
project(jacobi_async C)

set(CMAKE_C_STANDARD 11)

find_package(MPI REQUIRED)

include_directories(${MPI_C_INCLUDE_PATH})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MPI_C_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MPI_C_LINK_FLAGS}")
set(MPI_LIBRARIES ${MPI_C_LIBRARIES})

add_executable(
        jacobi_async
        main.c
        utils.h
        utils.c
        alg.c
        alg.h
)

target_compile_options(jacobi_async PUBLIC ${MPI_C_COMPILE_OPTIONS})
target_link_options(jacobi_async PUBLIC ${MPI_C_LINK_OPTIONS})
target_link_libraries(
        jacobi_async
        PUBLIC ${MPI_C_LIBRARIES}
        m
)
