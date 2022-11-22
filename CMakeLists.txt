cmake_minimum_required(VERSION 3.21)
project(RISC_V_simulator C)

set(CMAKE_C_STANDARD 11)

add_executable(RISC-V-simulator main.c functions.c functions.h)
