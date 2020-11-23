#!/bin/bash

set -e
PREFIX=/usr/local
cd $(dirname $0)/make
cmake .
make -j4
cd ..
cp -v make/lib/libhedge_hebase.a $PREFIX/lib/
mkdir -p $PREFIX/include/hedge/hebase/impl
mkdir -p $PREFIX/include/hedge/hebase/helib
mkdir -p $PREFIX/include/hedge/simple_nn
cp -v src/hedge/hebase/*.h $PREFIX/include/hedge/hebase/
cp -v src/hedge/hebase/impl/*.h $PREFIX/include/hedge/hebase/impl/
cp -v src/hedge/hebase/helib/*.h $PREFIX/include/hedge/hebase/helib/
cp -v src/hedge/simple_nn/*.h $PREFIX/include/hedge/simple_nn/
