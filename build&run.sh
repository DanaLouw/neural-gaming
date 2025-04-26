#!/bin/bash

$CURRENT_DIR=${PWD##*/}
echo $CURRENT_DIR

# Check if in scripts directory
if [ "$CURRENT_DIR" == "scripts" ]; then
  cd ..
fi

# Check if build directory exists
if [ ! -d "build" ]; then
  mkdir build
fi

# Check if saves.sqlite exists
if [ ! -e "./data/saves.sqlite3" ]; then
  touch ./data/saves.sqlite3
fi

# Builds the project
cmake -B build
make -C build

./build/Main
