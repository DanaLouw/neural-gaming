#!/bin/bash

$CURRENT_DIR=${PWD##*/}

# Check if in scripts directory
if [ "$CURRENT_DIR" == "scripts" ]; then
  cd ..
fi

# Check if build directory exists
if [ ! -d "build" ]; then
  mkdir build
fi

# Check if saves.sqlite exists
if [ ! -e "./data/saves.sqlite" ]; then
  touch ./data/saves.sqlite
fi

# Builds the project
cmake -B build
make -C build

./build/Main