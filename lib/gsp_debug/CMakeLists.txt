cmake_minimum_required(VERSION 3.10)

project(gsp_debug)

include_directories(include)

file(GLOB SOURCES "src/*.c")

add_library(gsp_debug OBJECT
 ${SOURCES}
)

target_include_directories(gsp_debug 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_types/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)