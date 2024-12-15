#!/bin/bash

# Navigate to the Vortex directory, configure, and build
cd Vortex
cmake -B build
cmake --build build --parallel 22
cmake --install build --prefix ./build/install

# Navigate to the Sandbox directory, configure, and build
cd ../Sandbox
cmake -B build
cmake --build build --parallel 22
