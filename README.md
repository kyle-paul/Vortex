# Vortex - 3D Real-time Rendering Engine

| | |
|-----|-----|
| ![](/Assets/imgs/physics.gif) | ![](/Assets/imgs/robot.gif) |

| |
|---|
| ![](/Assets/imgs/arms.gif) |


## Developer Guide
Clone all dependencies from git with correct version
```bash
git submodule update --init --recursive
```

Then set up development environment by running
```bash
chmod +x ./Scripts/SetupDevelopment.sh
./Scripts/SetupDevelopment.sh
```

Build the GLFW libs for linking into `libVortex`. If your Linux system is `WayLand`, remove the `GLFW_BUILD_WAYLAND=0` and replace with `GLFW_BUILD_X11=0`.

```bash
cmake -S Vortex/vendor/glfw \
      -B Vortex/libs/glfw-build \
      -D GLFW_BUILD_TESTS=OFF \
      -D GLFW_BUILD_EXAMPLES=OFF \
      -D GLFW_BUILD_DOCS=OFF \
      -D GLFW_BUILD_WAYLAND=0


cmake --build Vortex/libs/glfw-build --parallel <your-cpu-threads>
cmake --install Vortex/libs/glfw-build --prefix Vortex/libs/glfw-build
```

Build Glad support for OpenGL. Since Glad source code on Github can not be builts with CMake and installed for usage. Thus, you have to go to the download page for the released package of glad supporting the backend APIs in use.

Build the GLM math library libs

```bash
cmake -S Vortex/vendor/glm \
      -B Vortex/libs/glm-build \
      -D GLM_BUILD_TESTS=OFF \
      -D BUILD_SHARED_LIBS=OFF

cmake --build Vortex/libs/glm-build --parallel <your-cpu-threads> 
cmake --install Vortex/libs/glm-build --prefix Vortex/libs/glm-build
```

Build `Shaderc` library
```bash
cmake -S Vortex/vendor/shaderc \
      -B Vortex/libs/shaderc-build \
      -D CMAKE_BUILD_TYPE=Release

cmake --build Vortex/libs/shaderc-build --parallel <your-cpu-threads>
cmake --install Vortex/libs/shaderc-build --prefix Vortex/libs/shaderc-build
```

Build `Spirv-cross` library
```bash
cmake -S Vortex/vendor/spirv-cross \
      -B Vortex/libs/spirv-cross-build \
      -D CMAKE_POSITION_INDEPENDENT_CODE=ON

cmake --build Vortex/libs/spirv-cross-build --parallel <your-cpu-threads>
cmake --install Vortex/libs/spirv-cross-build --prefix Vortex/libs/spirv-cross-build
```
Build the Box3D library

```bash
cmake -S Vortex/vendor/box2d \
      -B Vortex/libs/box2d-build \
      -D GLFW_BUILD_WAYLAND=OFF \
      -D BOX2D_BUILD_DOCS=OFF

cmake --build Vortex/libs/box2d-build --parallel <your-cpu-threads>
cmake --install Vortex/libs/box2d-build --prefix Vortex/libs/box2d-build
```

Build YAML library
```bash
cmake -S Vortex/vendor/yaml-cpp \
      -B Vortex/libs/yaml-cpp-build \
      -D YAML_BUILD_SHARED_LIBS=OFF

cmake --build Vortex/libs/yaml-cpp-build --parallel <your-cpu-threads>
cmake --install Vortex/libs/yaml-cpp-build --prefix Vortex/libs/yaml-cpp-build
```

Build and install the Vortex engine that can be used in any other subprojects (e.g Sandbox)
```bash
cmake -B build
cmake --build build --parallel <your-cpu-threads>
cmake --install build --prefix <your-install-directory>
```

Since Vortex is currently built as dynamic library, in the Sandbox project or any other subprojects, you still need to find and include third party packages. Perhaps, in the future, Vortex can be improved for better usage experience later.

