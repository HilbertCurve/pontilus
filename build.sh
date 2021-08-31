#!/bin/bash

echo "Compiling..."
mkdir -p build

cd build

#IMPORTANT: compile engine first, then the rest; some things may break otherwise.
g++ -o game -I${PWD}/../include ../src/engine/*.cpp ../src/**.cpp -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl || exit -1
echo "Compiled"