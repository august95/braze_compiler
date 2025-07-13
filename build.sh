#!/bin/bash
echo "==> Cleaning previous build"
make clean

echo "==> Building project"
make

echo "==> Compiling test file with Braze"
./bin/braze_compiler
