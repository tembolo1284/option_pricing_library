#!/bin/bash
# scripts/run_tests.sh

# Ensure the script exits on any failure
set -e

# Run tests
mkdir -p build
cd build
cmake ..
make
ctest --output-on-failure

