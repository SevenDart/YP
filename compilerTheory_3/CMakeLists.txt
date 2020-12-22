cmake_minimum_required(VERSION 3.17)
project(compiler_theory_3 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS -fpic)

add_executable(compiler_theory_3 main.c analyzer.c analyzer.h analyzer.c)