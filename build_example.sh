g++ -o ./bin/$1 -g ./build/libpontilus.so ./examples/$1.cpp -I./src -I./src/vendor
./bin/$1
