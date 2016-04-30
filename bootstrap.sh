#!/bin/bash

if [ -d "build" ]; then
    rm -R ./build
fi

mkdir build

pushd build
cmake ../ -DCMAKE_TOOLCHAIN_FILE=../stm32f407_gcc.cmake -DCMAKE_BUILD_TYPE=Debug && make
popd

