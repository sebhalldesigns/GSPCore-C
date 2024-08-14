cmake_minimum_required(VERSION 3.10)

# Set the project name
project(GSPCore)

if(UNIX)

    message("Building GSPCore for UNIX/Linux")

    add_compile_definitions(PUBLIC GSPCORE_BUILD GSPCORE_BUILD_UNIX)

elseif(WIN32)

    message("Building GSPCore for WINDOWS")

    find_package(OpenGL REQUIRED)

    add_compile_definitions(PUBLIC GSPCORE_BUILD GSPCORE_BUILD_WIN32)
    #set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)  # Ensure all symbols are exported

    # Add subdirectories
    add_subdirectory(gsp_types)
    add_subdirectory(gsp_debug)
    add_subdirectory(gsp_window)
    add_subdirectory(gsp_list)
    #add_subdirectory(gsp_drawing)
    #add_subdirectory(gsp_views)
    #add_subdirectory(gsp_parsers)

    add_library(GSPCore SHARED
        $<TARGET_OBJECTS:gsp_types>
        $<TARGET_OBJECTS:gsp_debug>
        $<TARGET_OBJECTS:gsp_window>
        $<TARGET_OBJECTS:gsp_list>
    )

    target_link_libraries(GSPCore PUBLIC
        gdi32
        ${OPENGL_gl_LIBRARY}
        opengl32
        glu32
        d2d1
    )


endif()

