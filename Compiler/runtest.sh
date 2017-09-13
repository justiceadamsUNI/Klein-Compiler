#!/bin/bash
echo "Running Tests.  Please Wait."
g++ -std=gnu++11 -o RunTest test/*.cpp src/implementation/*.cpp
echo ""

./RunTest
