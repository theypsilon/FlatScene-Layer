#!/bin/bash

export CXX=g++
export CC=gcc

find -iname '*cmake*' -not -name CMakeLists.txt -exec rm -rf {} \+

git co CMake/

cmake .