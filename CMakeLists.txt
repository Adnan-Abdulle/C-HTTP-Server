cmake_minimum_required(VERSION 4.0)
project(COMP_4981_assign1 C)

set(CMAKE_C_STANDARD 11)

add_executable(COMP_4981_assign1 main.c
        read_write.c
        read_write.h
        socket.c
        socket.h
        errors.c
        errors.h)
