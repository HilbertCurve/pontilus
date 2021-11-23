#!/bin/bash

echo "Compiling test..."
mkdir -p build

cd build

g++ -o test -DGL_GLEXT_PROTOTYPES -I ../include ../src/*/*.cpp ../src/test.cpp -lGL -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl || exit -1

echo "Compiled"