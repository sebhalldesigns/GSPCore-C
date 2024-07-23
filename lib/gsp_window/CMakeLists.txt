cmake_minimum_required(VERSION 3.10)

project(gsp_window)

find_package(X11 REQUIRED)  

include_directories(include)

file(GLOB SOURCES "src/*.c")

add_library(gsp_window
 ${SOURCES}
)

target_include_directories(gsp_window 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_types/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_debug/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(gsp_window
    ${X11_LIBRARIES}
    X11
    gsp_debug
    wayland-client
)

