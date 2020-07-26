#!/bin/bash

set -e
PREFIX=/usr/local
cmake .
make -j4
cp -v lib/libsimple_ml_helib.a $PREFIX/lib/
mkdir -p $PREFIX/include/simple_ml_helib
cp -v src/*.h $PREFIX/include/simple_ml_helib/
