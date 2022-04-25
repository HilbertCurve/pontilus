#!/bin/bash

echo "Compiling..."
mkdir -p bin

cd bin

# IMPORTANT: compile packages first, then the rest; some things may break otherwise.
# Also, there should only ever be one layer in the package directory "tree"
g++ -o pontilus.so -DGL_GLEXT_PROTOTYPES -I../src -g ../src/*/*.cpp -shared -fPIC -lGL -lopenal -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl || exit -1

echo "Compiled"

