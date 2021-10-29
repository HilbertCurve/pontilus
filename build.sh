#!/bin/bash

echo "Compiling..."
mkdir -p build

cd build

# IMPORTANT: compile packages first, then the rest; some things may break otherwise.
# Also, there should only ever be one layer in the package directory "tree"
g++ -o game -DGL_GLEXT_PROTOTYPES -DMESA_GL_VERSION_OVERRIDE=3.3 -DMESA_GLSL_VERSION_OVERRIDE=330 -I../include -I../include/rapidjson -g ../src/*/*.cpp -g ../src/main.cpp -lGL -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl || exit -1
echo "Compiled"
