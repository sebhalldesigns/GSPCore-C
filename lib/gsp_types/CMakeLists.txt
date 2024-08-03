cmake_minimum_required(VERSION 3.10)

project(gsp_types)

include_directories(include)

file(GLOB SOURCES "src/*.c")

add_library(gsp_types OBJECT
 ${SOURCES}
)

target_include_directories(gsp_types 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)