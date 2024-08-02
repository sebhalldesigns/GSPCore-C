cmake_minimum_required(VERSION 3.10)

project(gsp_window)

find_package(X11 REQUIRED)  

include_directories(include)

if(EMSCRIPTEN)

elseif (UNIX)
    message("Setting up gsp_window on UNIX")

    add_library(gsp_window
        src/gsp_window.c
        src/gsp_window_x11.c
        src/gsp_window_wayland.c
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

elseif(WIN32)
    message("Setting up gsp_window on Windows")

    add_library(gsp_window
        src/gsp_window.c
        src/gsp_window_win32.c
    )

    target_include_directories(gsp_window 
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_types/include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_debug/include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    target_link_libraries(gsp_window
        gsp_debug
    )

else()
    message("Unknown platform" ${CMAKE_PLATFORM})
endif()

