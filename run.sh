#!/usr/bin/env bash

set -euo pipefail

mkdir build || true

cd build

cmake ..
make

cd ../demoark
../build/testdemoark
