cmake_minimum_required(VERSION 3.10)
project(Hazel LANGUAGES CXX)

# Specify the version of the library
set(Hazel_VERSION_MAJOR 1)
set(Hazel_VERSION_MINOR 0)

# Set the output directory for the Hazel library
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")

# Specify that Hazel is the shared library 
add_library(Hazel SHARED src/test.cpp)

# Include the header files in the build
target_include_directories(Hazel PUBLIC include)

# Set the compile definitions for Windows to handle DLL exports
target_compile_definitions(Hazel PRIVATE HAZEL_EXPORTS)