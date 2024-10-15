# Vortex - 3D Real-time Rendering Engine

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

Build the GLFW libs for linking into `libVortex`. If your Linux system is `WayLand`, remove the `GLFW_BUILD_WAYLAND=0` and replace with `GLFW_BUILD_X11=0`. For information can be found at the [GLFW documentation](https://www.glfw.org/docs/latest/compile_guide.html)

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