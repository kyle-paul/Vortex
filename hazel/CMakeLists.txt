cmake_minimum_required(VERSION 3.10)
project(Hazel LANGUAGES CXX)

# Set the output directory for the Hazel library
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")

# Specify that Hazel is the shared library
add_library(Hazel SHARED src/Application.cpp src/Logging.cpp)

# Include vendor directories
include_directories(Hazel vendor/spdlog/include)

# Include the header files in the build
target_include_directories(Hazel PUBLIC include)