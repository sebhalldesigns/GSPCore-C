cmake_minimum_required(VERSION 3.10)

project(gsp_window)

include_directories(include)

if(EMSCRIPTEN)

elseif (UNIX)
    message("Setting up gsp_window on UNIX")

    find_package(X11 REQUIRED)  

    add_library(gsp_window
        src/gsp_window.c
        src/gsp_window_x11.c
        src/gsp_window_wayland.c
    )

    target_include_directories(gsp_window 
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_types/include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_debug/include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_list/include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_renderer/include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    target_link_libraries(gsp_window
        ${X11_LIBRARIES}
        X11
        gsp_debug
        gsp_list
        gsp_renderer
        wayland-client
    )

elseif(WIN32)
    message("Setting up gsp_window on Windows")

    find_package(OpenGL REQUIRED)

    add_library(gsp_window OBJECT
        src/gsp_window.c
        src/gsp_window_win32.c
    )

    target_include_directories(gsp_window 
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_types/include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_debug/include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_list/include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gsp_renderer/include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    target_link_libraries(gsp_window
        gdi32
        ${OPENGL_gl_LIBRARY}
        opengl32
        glu32
        gsp_debug
        gsp_list
        gsp_renderer
    )

else()
    message("Unknown platform" ${CMAKE_PLATFORM})
endif()

