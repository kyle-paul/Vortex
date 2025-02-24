#!/bin/bash

# List of vendor directories and their corresponding tag versions
declare -a vendors=("spdlog" "glfw", "imgui", "glm", "shaderc", "spirv-cross", "yaml-cpp", "gizmo", "imnode", "box2d", "bullet3")
declare -a versions=("v1.14.1" "3.4", "v1.91.3-docking", "1.0.1", "v2024.3". "vulkan-sdk-1.3.296.0", "0.8.0", "1.83", "v0.9.3", "v3.0.0", "3.25")

# Initialize and update submodules
git submodule update --init --recursive

# Iterate over the vendor directories and checkout the respective tags
for i in "${!vendors[@]}"; do
    vendor="${vendors[$i]}"
    version="${versions[$i]}"
    echo "Checking out $vendor to tag $version"
    
    cd Vortex/vendor/$vendor || exit
    git checkout tags/$version
    
    # Return to the root directory
    cd - || exit
done

echo "Setup completed."
