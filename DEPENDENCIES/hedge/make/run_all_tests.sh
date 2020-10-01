#!/bin/bash

set -e

x=$1

if [ "$x" != "--no_make" ]; then
make
fi

bin/helib_tests
bin/helib_bgv_tests 

echo "All tests passed"