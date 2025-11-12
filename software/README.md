# Software

## Dependencies
- CMake
- GCC or other C compiler
- pico-sdk for C/C++ installed locally (with necessary modules)

## Build
First, make sure your OS recognizes the path to pico-sdk. On Linux/MacOS:
```
export PICO_SDK_PATH=/your/path/to/pico-sdk
```

From the project directory, create a build directory and from it run the .cmake file:
```
mkdir build
cd build
cmake ..
```

If succesful, run the Makefile from build directory:
```
make
```

