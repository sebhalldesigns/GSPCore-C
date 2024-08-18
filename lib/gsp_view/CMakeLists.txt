cmake_minimum_required(VERSION 3.10)

project(gsp_view)

include_directories(include)

message("Setting up gsp_view - CROSS-PLATFORM")

add_library(gsp_view OBJECT
    src/gsp_view.c
)

target_include_directories(gsp_view 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_types/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_debug/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_containers/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_renderer/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

