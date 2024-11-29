#!/bin/bash
# scripts/build_and_run.sh

# Ensure the script exits on any failure
set -e

# Build the project
mkdir -p build
cd build
cmake ..
make

# Run the server
echo "Starting server..."
./bin/PricingServer

