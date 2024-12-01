#!/bin/bash
# scripts/run_tests.sh

# Ensure the script exits on any failure
set -e

# Run tests
cmake -B build -G Ninja
cmake --build build
cd build
ctest --output-on-failure -V

