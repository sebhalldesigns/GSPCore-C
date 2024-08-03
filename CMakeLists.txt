cmake_minimum_required(VERSION 3.10)

# Set the project name
project(GSPCoreTest)

set(CMAKE_GENERATOR_PLATFORM x64)

add_compile_definitions(DEBUG)

# Add the subdirectories for libraries and tests
add_subdirectory(lib)
#add_subdirectory(test)