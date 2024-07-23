cmake_minimum_required(VERSION 3.10)

project(gsp_drawing)

include_directories(include)

file(GLOB SOURCES "src/*.c")

add_library(gsp_drawing
 ${SOURCES}
)

target_include_directories(gsp_drawing 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_types/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_debug/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(gsp_drawing

)

