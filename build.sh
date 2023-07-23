#!/bin/bash

set -e
set -x

function clean() {
    rm -rf build
    rm -rf bindings 
    mkdir build
    pushd build
    export CONAN_SYSREQUIRES_MODE=enabled
    conan install .. --build=missing -s compiler.libcxx=libstdc++11 -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True
    # cmake -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON .. 
    cmake .. 
    popd
}

if [ $# -gt 0 ]; then
    if [ $1 == "clean" ]; then
       clean
    fi
fi

cmake --build ./build
cp etc/* ./build/etc
