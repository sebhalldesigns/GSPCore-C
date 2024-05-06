cmake_minimum_required(VERSION 3.5)

project(GSPCore)

# Set C standard to C99
set(CMAKE_C_STANDARD 99)


message("\nBuilding GSPCore")

add_compile_definitions(DEBUG)

if (UNIX)

    message("Building GSPCore for UNIX/Linux")

    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

    # Find X11 library
    find_package(X11 REQUIRED)
    find_package(Cairo REQUIRED)

    set(GSPCORE_SRC
        src/platform/x11/GSPCore_x11.c
        src/platform/x11/GWindow_x11.c
        src/GApplication.c
        src/GVector.c
        src/GLog.c
        src/graphics/unix/GFrame_gl.c
        src/graphics/unix/GVertexBuffer_gl.c
        src/graphics/unix/GShader_gl.c
        src/graphics/unix/GTexture_gl.c
        src/graphics/unix/unix_GTextProvider.c
    )

    # Set the C compiler flags
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

    add_library(GSPCore STATIC ${GSPCORE_SRC})

    target_include_directories(GSPCore PUBLIC ${CAIRO_INCLUDE_DIRS}  ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})

    # Link X11 library
    target_link_libraries(GSPCore ${X11_LIBRARIES} GL GLEW m ${CAIRO_LIBRARIES})

elseif(WIN32)

    message("Building GSPCore for Microsoft Windows")
    set(CMAKE_GENERATOR_PLATFORM x64)

    set(GSPCORE_SRC
        src/platform/win32/win32_GSPCore.c
        src/GApplication.c
        src/GVector.c
        src/GLog.c
        src/platform/win32/GWindow.c
        src/graphics/win32/GTextProvider.c
    )

    add_library(GSPCore STATIC ${GSPCORE_SRC})
    target_include_directories(GSPCore PUBLIC ${CAIRO_INCLUDE_DIRS}  ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(GSPCore PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/glew32s.lib opengl32 gdi32)

else()
    message("Unknown platform" ${CMAKE_PLATFORM})
endif()







#add_subdirectory(libraries)


# Add executable target
add_executable(Test ${CMAKE_CURRENT_SOURCE_DIR}/testing/main.c)
target_link_libraries(Test PRIVATE GSPCore)

target_include_directories(Test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libraries/GSPCore/include)