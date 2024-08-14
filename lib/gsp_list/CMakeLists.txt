cmake_minimum_required(VERSION 3.10)

project(gsp_list)

include_directories(include)

message("Setting up gsp_list - CROSS-PLATFORM")

add_library(gsp_list OBJECT
    src/gsp_list.c
)

target_include_directories(gsp_list 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_types/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_debug/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

