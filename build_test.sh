#!/bin/bash

./build.sh

g++ -o bin/$1 -I./src bin/pontilus.so tests/$1.cpp -DGL_GLEXT_PROTOTYPES -g -shared -fPIC -lGL -lopenal -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl 

./bin/$1 || exit -1

echo "Test $1 ran successfully."

