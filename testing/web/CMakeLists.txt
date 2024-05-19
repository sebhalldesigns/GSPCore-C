cmake_minimum_required(VERSION 3.5)

project(MyEmscriptenProject)

set(CMAKE_C_COMPILER emcc)

set(CMAKE_CXX_COMPILER em++)
set(CMAKE_EXECUTABLE_SUFFIX ".html")

add_executable(MyEmscriptenProject main.c)