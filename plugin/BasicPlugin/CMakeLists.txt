cmake_minimum_required(VERSION 3.16)

project(BasicPlugin CXX)
set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
# COMPILE OPTIONS
add_compile_options(-Wall -pthread -lportaudio -lasound -lm -lpthread)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)

