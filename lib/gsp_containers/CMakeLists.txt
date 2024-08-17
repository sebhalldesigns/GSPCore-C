cmake_minimum_required(VERSION 3.10)

project(gsp_containers)

include_directories(include)

message("Setting up gsp_containers - CROSS-PLATFORM")

add_library(gsp_containers OBJECT
    src/gsp_list.c
    src/gsp_tree.c
)

target_include_directories(gsp_containers 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_types/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_debug/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

