#!/bin/bash

echo "Compiling..."
mkdir -p bin

cd bin

# IMPORTANT: compile packages first, then the rest; some things may break otherwise.
# Also, there should only ever be one layer in the package directory "tree"
g++ -o game -DGL_GLEXT_PROTOTYPES -I../src -I../include/rapidjson -g ../src/*/*.cpp ../src/main.cpp -lGL -lopenal -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl || exit -1
echo "Compiled"
