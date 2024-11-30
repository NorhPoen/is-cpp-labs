cmake_minimum_required(VERSION 3.24)
project(cyclebuffer)

set(CMAKE_CXX_STANDARD 17)

add_executable(cyclebuffer main.cpp BufferIterator.h BufferIteratorConst.h CCircularBuffer.h CCircularBufferExt.h)

enable_testing()
add_subdirectory(tests)
