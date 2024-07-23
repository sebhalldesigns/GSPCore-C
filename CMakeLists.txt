cmake_minimum_required(VERSION 3.10)

# Set the project name
project(GSPCoreTest)

add_compile_definitions(DEBUG)

# Add the subdirectories for libraries and tests
add_subdirectory(lib)
add_subdirectory(test)