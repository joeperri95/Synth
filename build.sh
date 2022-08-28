#!/bin/bash

set -e
set -x

rm -rf build
rm -rf bindings 
mkdir build
pushd build

export CONAN_SYSREQUIRES_MODE=enabled
conan install .. --build=missing -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True
cmake .. 
cmake --build .
