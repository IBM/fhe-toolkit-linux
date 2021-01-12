#!/bin/bash

set -e

echo "Running ML-HElib unit tests"
cd /opt/IBM/FHE-distro/ML-HElib/make
mkdir -p /tmp/output
HELAYERS_TESTS_OUTPUT_DIR=/tmp/output ./run_all_tests.sh --no_make

echo "Testing sample BGV_world_country_db_lookup"
cd /opt/IBM/FHE-Workspace/examples/BGV_world_country_db_lookup
mkdir -p build
cd build
cmake ..
make
cd ..
./runtest.sh

echo "Testing sample mlhelib_tutorial"
cd /opt/IBM/FHE-Workspace/examples/mlhelib_tutorial
mkdir -p build
cd build
cmake ..
make
./mlhelib_tutorial basics
./mlhelib_tutorial plaintexts
./mlhelib_tutorial io
./mlhelib_tutorial integers

echo "Testing sample CKKS_credit_card_fraud"
cd /opt/IBM/FHE-Workspace/examples/CKKS_credit_card_fraud
mkdir -p build
cd build
cmake ..
make
./CKKS_credit_card_fraud

echo "ML-HElib tests done"
