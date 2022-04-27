#!/bin/bash

mkdir tmp
cd tmp
git clone https://github.com/nlohmann/json

mkdir /usr/local/include/nlohmann
cp json/single_include/nlohmann/json.hpp /usr/local/include/nlohmann/json.hpp

cd ..
rm -r tmp

