cmake_minimum_required(VERSION 3.5)

project(main)

include_directories(
    ${CMAKE_SOURCE_DIR}/lib/gsp_window/include
    ${CMAKE_SOURCE_DIR}/lib/gsp_debug/include
)

add_executable(main main.c)
target_link_libraries(main gsp_window)