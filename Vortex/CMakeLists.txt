cmake_minimum_required(VERSION 3.10)
project(Vortex LANGUAGES CXX)

# Set C++ standard version
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "${_msg} - C++${CMAKE_CXX_STANDARD}")

if(NOT CMAKE_CXX_STANDARD MATCHES "^(17|20)$")
	message(FATAL_ERROR "CMAKE_CXX_STANDARD must be set to 17 or 20")
endif()

# Prevent unnecessary re-linking on Linux 
if(NOT DEFINED CMAKE_LINK_DEPENDS_NO_SHARED AND UNIX AND NOT APPLE)
	set(CMAKE_LINK_DEPENDS_NO_SHARED ON)
endif()

# Set the output directory for the Vortex library
set(BUILD_INSTANT_LIBRARY OFF)
if (BUILD_INSTANT_LIBRARY)
	message(STATUS "BUILD_INSTANT_LIBRARY is set to ${BUILD_INSTANT_LIBRARY}")
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
else()
	message(STATUS "BUILD_INSTANT_LIBRARY is set to ${BUILD_INSTANT_LIBRARY}")
endif()


# Set the root directory for your project files
set(GIZMO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/vendor/gizmo")

# Gather all .h and .cpp files in variables
file(GLOB_RECURSE GIZMO_HEADER_FILES "${GIZMO_ROOT}/*.h")
file(GLOB_RECURSE GIZMO_SOURCE_FILES "${GIZMO_ROOT}/*.cpp")

# Specify that Vortex is the shared library
add_library(${PROJECT_NAME} SHARED 
	# Vortex source
	src/Application.cpp
	src/Logging.cpp
	src/PreHeaders.cpp
	src/LinuxWindow.cpp
	src/LinuxPlatformUtils.cpp
	src/WIndowInput.cpp
	src/Layer.cpp
	src/LayerStack.cpp
	src/LayerStack.cpp
	
	# ImGui internal sources
	src/ImGuiOpenGLRenderer.cpp
	src/ImGuiGlfwRender.cpp
	src/ImGuiLayerBind.cpp
	src/ImGuiLayer.cpp
	src/GraphicsContext.cpp
	src/OpenGLGraphicsContext.cpp
	src/Renderer.cpp
	src/Renderer2D.cpp
	src/RenderCommand.cpp
	src/RendererAPI.cpp
	src/OpenGLRendererAPI.cpp
	src/Shader.cpp
	src/OpenGLShader.cpp
	src/Buffer.cpp
	src/OpenGLBuffer.cpp
	src/VertexArray.cpp
	src/OpenGLVertexArray.cpp
	src/OrthographicCamera.cpp
	src/OrthographicCameraController.cpp
	src/Texture.cpp
	src/OpenGLTexture2D.cpp
	src/Framebuffer.cpp
	src/OpenGLFramebuffer.cpp
	src/SceneSerializer.cpp
	src/EditorCamera.cpp
	src/MeshRenderer.cpp

	# Entity components
	src/Scene.cpp
	src/Entity.cpp
	src/SceneCamera.cpp
	src/Math.cpp
	src/Mesh.cpp
	src/Shape.cpp

	# Vendor sources
	vendor/glad/src/glad.c
	vendor/stb/stb_image.cpp
	vendor/imgui/imgui.cpp
	vendor/imgui/imgui_demo.cpp
	vendor/imgui/imgui_draw.cpp
	vendor/imgui/imgui_tables.cpp
	vendor/imgui/imgui_widgets.cpp

	${GIZMO_SOURCE_FILES}
)

# Precompiled headers
set(PRECOMPILED_HEADERS ON)
if (PRECOMPILED_HEADERS)
	message(STATUS "PRECOMPILED_HEADERS is turned ${PRECOMPILED_HEADERS}")
	target_precompile_headers(Vortex PUBLIC include/Vortex/PreHeaders.h)
else()
	message(STATUS "PRECOMPILED_HEADERS is turned ${PRECOMPILED_HEADERS}")
endif()

# Include vendor directories
include_directories(${PROJECT_NAME} 
	vendor/spdlog/include
	vendor/glad/include
	vendor/imgui
	vendor/stb
	vendor/entt/include
	libs/glfw-build/include/GLFW
	libs/glm-build/include/glm
	libs/shaderc-build/include
	libs/spirv-cross-build/include
	libs/yaml-cpp-build/include
	${GIZMO_ROOT}
)

# Include built packages
set(SHADERC_LIB ${CMAKE_CURRENT_SOURCE_DIR}/libs/shaderc-build/lib/libshaderc_combined.a)
set(YAML_LIB ${CMAKE_CURRENT_SOURCE_DIR}/libs/yaml-cpp-build/lib/libyaml-cpp.a)
set(SPIRV_CROSS_LIBS 
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/spirv-cross-build/libspirv-cross-core.a 
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/spirv-cross-build/libspirv-cross-cpp.a 
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/spirv-cross-build/libspirv-cross-glsl.a
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/spirv-cross-build/libspirv-cross-hlsl.a
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/spirv-cross-build/libspirv-cross-msl.a
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/spirv-cross-build/libspirv-cross-reflect.a
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/spirv-cross-build/libspirv-cross-util.a)

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
target_include_directories(${PROJECT_NAME} 
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Link dependencies into my lib
target_link_libraries(
		${PROJECT_NAME} PRIVATE 
		glfw glm::glm ${OPENGL_LIBRARIES} 
		${SHADERC_LIB} ${SPIRV_CROSS_LIBS} ${YAML_LIB})

# Installation
set(INSTALL_BINDIR "bin")
set(INSTALL_LIBDIR "lib")
set(INSTALL_INCLUDEDIR "include/${PROJECT_NAME}")

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    RUNTIME DESTINATION ${INSTALL_BINDIR}    # for .exe or .dll
    LIBRARY DESTINATION ${INSTALL_LIBDIR}    # for shared libraries
    ARCHIVE DESTINATION ${INSTALL_LIBDIR}    # for static libraries
)

install(DIRECTORY include/ DESTINATION ${INSTALL_INCLUDEDIR})

# Install cmake config files for find_package
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# Install header files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION 1.0
    COMPATIBILITY AnyNewerVersion
)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    @ONLY
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
