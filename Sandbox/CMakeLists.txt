cmake_minimum_required(VERSION 3.10)
project(Sandbox)

set(ENGINE_NAME Vortex)
set(${ENGINE_NAME}_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../${ENGINE_NAME})

# Add the executable and source files
add_executable(Sandbox 
	src/SandboxApp.cpp 
	${${ENGINE_NAME}_DIR}/vendor/glad/src/glad.c
)

target_include_directories(Sandbox PRIVATE
	${${ENGINE_NAME}_DIR}/build/install/include/Vortex
	${${ENGINE_NAME}_DIR}/vendor/spdlog/include
	${${ENGINE_NAME}_DIR}/vendor/glad/include
	${${ENGINE_NAME}_DIR}/vendor/imgui
	${${ENGINE_NAME}_DIR}/libs/glfw-build/include
	${${ENGINE_NAME}_DIR}/libs/glm-build/include
)

target_link_libraries(Sandbox ${${ENGINE_NAME}_DIR}/build/install/lib/libVortex.so)

list(APPEND CMAKE_PREFIX_PATH ${${ENGINE_NAME}_DIR}/libs/glfw-build/lib/cmake/glfw3)
list(APPEND CMAKE_PREFIX_PATH ${${ENGINE_NAME}_DIR}/libs/glm-build/share/glm)
list(APPEND CMAKE_PREFIX_PATH ${${ENGINE_NAME}_DIR}/build/install/lib/cmake/Vortex)

find_package(glfw3 REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Vortex REQUIRED)