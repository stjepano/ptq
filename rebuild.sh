#!/bin/sh

set -xe

cmake -B cmake-build-debug/ -S . -DCMAKE_BUILD_TYPE=Debug

cmake --build cmake-build-debug/
