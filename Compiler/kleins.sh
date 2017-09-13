#!/bin/bash
KLEINFILE=$1

echo "Compiling program.  Please Wait."
g++ -std=gnu++11 -o BuildTokens src/BuildTokens.cpp src/implementation/*.cpp

./BuildTokens $KLEINFILE
