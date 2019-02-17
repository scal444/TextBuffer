cmake_minimum_required(VERSION 3.10)
project(TextBuffer)

set(CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -o0")


add_executable(TextBufferExample stringutil.cpp exampleUsage.cpp TextBuffer.cpp)

enable_testing()
add_executable(TextBufferTests testTextBuffer.cpp TextBuffer.cpp stringutil.cpp)
add_test(textbuffertest TextBufferTests)


