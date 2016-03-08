#!/bin/bash

rm -f *.deb
rm -rf workspace
mkdir workspace
pushd workspace > /dev/null
cmake -DCMAKE_VERBOSE_MAKEFILE=OFF -DCPACK_GENERATOR=DEB ..
make package
popd > /dev/null
