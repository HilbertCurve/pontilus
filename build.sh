#!/bin/bash

echo "Compiling..."
mkdir -p build

cd build
g++ -o game ../src/*.cpp -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl || exit -1
echo "Compiled"