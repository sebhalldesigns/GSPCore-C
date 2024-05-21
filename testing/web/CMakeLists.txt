cmake_minimum_required(VERSION 3.5)

project(MyEmscriptenProject)

# compile with emcmake cmake .. then emmake make

set(CMAKE_TOOLCHAIN_FILE "$ENV{EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")


add_executable(MyEmscriptenProject main.c)

set(CMAKE_EXECUTABLE_SUFFIX ".js")
set_target_properties(MyEmscriptenProject PROPERTIES OUTPUT_NAME "index")

set_target_properties(MyEmscriptenProject PROPERTIES LINK_FLAGS "-s EXPORTED_RUNTIME_METHODS=['ccall','cwrap']")

# Copy custom HTML file to the output directory
add_custom_command(
    TARGET MyEmscriptenProject POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/gspcore.html ${CMAKE_BINARY_DIR}/index.html
)