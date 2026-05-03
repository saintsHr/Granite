#!/usr/bin/env bash

BUILD_DIR="build"

if [ ! -d "$BUILD_DIR" ]; then
    echo "Build directory not found. Run cmake first."
    exit 1
fi

echo "Installing..."
sudo cmake --install "$BUILD_DIR"

echo "Done."
