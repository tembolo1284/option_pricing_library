#!/bin/bash
# scripts/build_and_run.sh

# Ensure the script exits on any failure
rm -rf build
set -e

# Build the project
cmake -B build -G Ninja
cmake --build build -j1

# Run the server
echo "Starting server..."
./build/bin/PricingServer

