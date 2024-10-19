#!/bin/bash

set -e
set -x

function clean() {
    rm -rf build
    rm -rf bindings 
    mkdir build
    conan install . --output-folder=build --build=missing -c tools.system.package_manager:mode=install -s build_type=Debug
    cmake -S . -B ./build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
}

if [ $# -gt 0 ]; then
    if [ $1 == "--clean" ]; then
        echo "test"
        clean
    fi
fi

cmake --build ./build -j 8
cp etc/* ./build/etc
cp assets/wav/* ./build
