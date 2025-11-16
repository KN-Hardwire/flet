# Software

## Dependencies
- CMake
- GCC or other C compiler
- pico-sdk for C/C++ installed locally (with necessary modules)

## Build
Initialize pico-sdk and its submodules:
```
git submodule update --init --recursive
```

Create a build directory and from it run cmake: 
```
mkdir build
cd build
cmake ..
```

If succesful, run the Makefile from build directory:
```
make
```

