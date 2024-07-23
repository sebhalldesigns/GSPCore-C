cmake_minimum_required(VERSION 3.10)

# Set the project name
project(GSPCore)

if(UNIX)

    message("Building GSPCore for UNIX/Linux")

    add_compile_definitions(PUBLIC GSPCORE_BUILD GSPCORE_BUILD_UNIX)



endif()

# Add subdirectories
add_subdirectory(gsp_types)
add_subdirectory(gsp_debug)
add_subdirectory(gsp_window)
add_subdirectory(gsp_drawing)
#add_subdirectory(gsp_views)
#add_subdirectory(gsp_parsers)
