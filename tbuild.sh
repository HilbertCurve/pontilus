#!/bin/bash

echo "Compiling test..."
mkdir -p build

cd build

g++ -o test -I ../include ../src/test.cpp

echo "Compiled"