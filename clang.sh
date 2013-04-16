#!/bin/bash

export CXX=clang++
export CC=clang

find -iname '*cmake*' -not -name CMakeLists.txt -exec rm -rf {} \+

git co CMake/

cmake .