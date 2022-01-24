#!/bin/bash

echo "Compiling test..."
mkdir -p build

cd build

g++ -o test -DGL_GLEXT_PROTOTYPES -g -I ../include ../src/*/*.cpp ../src/test.cpp -lGL -lopenal -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl || exit -1

echo "Compiled"