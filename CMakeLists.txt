cmake_minimum_required(VERSION 3.10)
project(TextBuffer)

set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -o0")

file( GLOB "*.cpp")

add_library(TextBufLib TextBuffer.cpp)
add_executable(TextBufferExample TextBuffer.cpp main.cpp)

enable_testing()
add_executable(TextBufferTests TextBuffer.cpp testTextBuffer.cpp)
add_test(textbuffertest TextBufferTests)


