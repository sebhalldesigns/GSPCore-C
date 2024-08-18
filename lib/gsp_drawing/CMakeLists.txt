cmake_minimum_required(VERSION 3.10)

project(gsp_drawing)

include_directories(include)

add_library(gsp_drawing OBJECT
 src/gsp_drawing.c
 src/gsp_drawing_d2d1.cpp
)

target_include_directories(gsp_drawing 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_types/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_debug/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(gsp_drawing
    gsp_debug
    d2d1
    dwrite
    windowscodecs
)

