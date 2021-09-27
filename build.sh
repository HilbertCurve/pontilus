#!/bin/bash

echo "Compiling..."
mkdir -p build

cd build

# IMPORTANT: compile packages first, then the rest; some things may break otherwise.
# Also, there should only ever be one layer in the package directory "tree"
g++ -Wall -o game -I../include ../src/*/*.cpp ../src/main.cpp -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl || exit -1
echo "Compiled"