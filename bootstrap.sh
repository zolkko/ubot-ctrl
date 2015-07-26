#!/bin/bash

mkdir build
cd build
cmake ../ -DCMAKE_TOOLCHAIN_FILE=../stm32f407_gcc.cmake -DCMAKE_BUILD_TYPE=Debug
make

