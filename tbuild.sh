#!/bin/bash

echo "Compiling test..."
mkdir -p build

cd build

g++ -o test -I ../include ../src/utils/*.cpp ../src/test.cpp

echo "Compiled"