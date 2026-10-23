#!/bin/bash 
cmake -S . -B build && cmake --build build 
export LD_LIBRARY_PATH="$(pwd)/build/bin:$LD_LIBRARY_PATH"
sen run config.yaml
