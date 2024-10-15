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
	src/WIndowInput.cpp

	src/Layer.cpp
	src/LayerStack.cpp
	
	src/ImGuiOpenGLRenderer.cpp
	# src/ImGuiGlfwRender.cpp
	src/ImGuiLayerBind.cpp
	src/ImGuiLayer.cpp

	vendor/glad/src/glad.c
	vendor/imgui/imgui.cpp
	vendor/imgui/imgui_demo.cpp
	vendor/imgui/imgui_draw.cpp
	vendor/imgui/imgui_tables.cpp
	vendor/imgui/imgui_widgets.cpp
	vendor/imgui/backends/imgui_impl_glfw.cpp
)

# Precompiled headers
target_precompile_headers(Vortex PUBLIC include/Vortex/PreHeaders.h)

# Include vendor directories
include_directories(${PROJECT_NAME} 
	vendor/spdlog/include
	vendor/glad/include
	vendor/imgui
	libs/glfw-build/include/GLFW
	libs/glm-build/include/glm
)

# Include built packages
list(APPEND CMAKE_PREFIX_PATH libs/glfw-build/lib/cmake/glfw3)
list(APPEND CMAKE_PREFIX_PATH libs/glm-build/share/glm)

find_package(glfw3 REQUIRED)
find_package(glm CONFIG REQUIRED)

# Set policy to modern GLVND and find OpenGL
if (POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()
find_package(OpenGL REQUIRED)

# Include the header files in the build
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Link dependencies into my lib
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm::glm ${OPENGL_LIBRARIES})