cmake_minimum_required(VERSION 4.0)
project(labaC C)

set(CMAKE_C_STANDARD 23)

#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/input.txt
#        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(labaC с1-1.c
        linked_list.c)
