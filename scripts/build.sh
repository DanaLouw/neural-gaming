#!/bin/bash
$DIRECTORY=build
$CURRENT_DIR=${PWD##*/}

if [ "$CURRENT_DIR" == "scripts" ]; then
  cd ..
fi

if [ ! -d "$DIRECTORY" ]; then
  mkdir build
fi

cmake -B build
make build