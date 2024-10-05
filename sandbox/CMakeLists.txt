cmake_minimum_required(VERSION 3.10)
project(Sandbox)

set(HAZEL_LIB /home/pc/dev/engine/hazel/lib)

# Add the executable and source files
add_executable(Sandbox src/SandboxApp.cpp)

# Include directories (for public headers)
target_include_directories(Sandbox PRIVATE ../hazel/include ../hazel/vendor/spdlog/include)

# Link the Hazel library to the sandbox executable
target_link_libraries(Sandbox ${HAZEL_LIB}/libHazel.so)