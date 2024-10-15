cmake_minimum_required(VERSION 3.10)
project(Sandbox)

set(ENGINE_NAME Vortex)
set(VORTEX_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../${ENGINE_NAME}/bin)

# Add the executable and source files
add_executable(Sandbox 
	src/SandboxApp.cpp 
	../${ENGINE_NAME}/vendor/glad/src/glad.c
)

# Include directories (for public headers)
target_include_directories(Sandbox PRIVATE 
	../${ENGINE_NAME}/include
	../${ENGINE_NAME}/vendor/spdlog/include
	../${ENGINE_NAME}/vendor/glad/include
	../${ENGINE_NAME}/libs/glfw-build/include
)

# Link the Hazel library to the sandbox executable
target_link_libraries(Sandbox ${VORTEX_LIB}/libVortex.so)