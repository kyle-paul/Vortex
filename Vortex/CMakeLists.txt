cmake_minimum_required(VERSION 3.10)
project(Vortex LANGUAGES CXX)

# Set the output directory for the Vortex library
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# Specify that Vortex is the shared library
add_library(${PROJECT_NAME} SHARED 
	src/Application.cpp
	src/Logging.cpp
	src/PreHeaders.cpp
	src/LinuxWindow.cpp
)

# Precompiled headers
target_precompile_headers(Vortex PUBLIC include/Vortex/PreHeaders.h)

# Include vendor directories
include_directories(${PROJECT_NAME} 
	vendor/spdlog/include 
	libs/glfw-build/include/GLFW
)

# Check dependencies
list(APPEND CMAKE_PREFIX_PATH libs/glfw-build/lib/cmake/glfw3)
find_package(glfw3 REQUIRED)

# Include the header files in the build
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Link dependencies into my lib
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)