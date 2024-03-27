cmake_minimum_required(VERSION 3.27)
project(SR02 C)

set(CMAKE_C_STANDARD 11)

include_directories(TD2)

add_executable(SR02
        exo1.c,
        exo2.c,
        exo3-projet.c
)
