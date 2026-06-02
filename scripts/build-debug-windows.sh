#!/bin/bash

set -e
cmake -B build \
    -DCMAKE_TOOLCHAIN_FILE="$HOME/vcpkg/scripts/buildsystems/vcpkg.cmake" \
    -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE="$(pwd)/cmake/toolchains/mingw-w64-x86_64.cmake" \
    -DVCPKG_TARGET_TRIPLET=x64-mingw-static \
    -DCMAKE_PREFIX_PATH="$HOME/vcpkg/installed/x64-mingw-static" \
    -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
