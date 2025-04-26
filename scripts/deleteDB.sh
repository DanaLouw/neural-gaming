#!/bin/bash

$CURRENT_DIR=${PWD##*/}

# Check if in scripts directory
if [ "$CURRENT_DIR" == "scripts" ]; then
  cd ..
fi

if [ -e "./data/saves.sqlite3" ]; then
  rm -r ./data/saves.sqlite3
  echo "Database deleted"
fi