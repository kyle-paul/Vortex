cmake_minimum_required(VERSION 3.10)
project(Vortex LANGUAGES CXX)

# Set the output directory for the Vortex library
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")

# Specify that Vortex is the shared library
add_library(${PROJECT_NAME} SHARED 
	src/Application.cpp
	src/Logging.cpp
	src/PreHeaders.cpp
)

# Include vendor directories
include_directories(${PROJECT_NAME} 
	vendor/spdlog/include 
	vendor/glfw/include
)

# Include the header files in the build
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Precompiled headers
target_precompile_headers(Vortex PUBLIC include/Vortex/PreHeaders.h)