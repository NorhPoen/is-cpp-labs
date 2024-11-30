cmake_minimum_required(VERSION 3.24)
project(labwork_9_ISKorobov)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(labwork_9_ISKorobov main.cpp Allocator.h Pool.h)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Config.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

