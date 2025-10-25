cmake_minimum_required(VERSION 4.0)
project(calculatorV2 C)

set(CMAKE_C_STANDARD 23)

add_executable(calculatorV2 main.c
        calculator.h
        calculator.c)
