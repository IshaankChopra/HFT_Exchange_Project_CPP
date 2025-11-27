#!/bin/bash

# Stop on first error
set -e

BUILD_DIR="./build"

echo "🔨 Starting Build Process..."

# Create Build Directory
if [ ! -d "$BUILD_DIR" ]; then
    echo "   Creating build directory..."
    mkdir -p $BUILD_DIR
fi

# Run CMake and Make
cd $BUILD_DIR
echo "   Configuring with CMake..."
cmake .. > /dev/null

echo "   Compiling..."
make -j4

echo "✅ Build Complete. Executables are in $BUILD_DIR/"