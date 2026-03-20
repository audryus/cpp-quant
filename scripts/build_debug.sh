#!/bin/bash
cd "$(dirname "$0")/.."
cmake -S . -B build_debug -DCMAKE_BUILD_TYPE=Debug -G Ninja
cmake --build build_debug -- -j$(nproc)
