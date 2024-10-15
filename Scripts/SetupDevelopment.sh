#!/bin/bash

# List of vendor directories and their corresponding tag versions
declare -a vendors=("spdlog" "glfw", "imgui")
declare -a versions=("v1.14.1" "3.4", "v1.91.3-docking")

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
