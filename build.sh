#!/bin/bash

rm -rf build
mkdir build
pushd build > /dev/null
cmake -DCMAKE_VERBOSE_MAKEFILE=OFF ..
make
popd > /dev/null
