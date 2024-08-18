cmake_minimum_required(VERSION 3.10)

project(gsp_renderer)

include_directories(include)

message("Setting up gsp_renderer - CROSS-PLATFORM")

add_library(gsp_renderer OBJECT
    src/gsp_renderer.c
    src/glad.c
)

target_include_directories(gsp_renderer 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_types/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_debug/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_containers/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_drawing/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)


