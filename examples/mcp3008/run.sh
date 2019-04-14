#!/bin/bash

make
LIBDIR=`pwd`/../../build
LD_LIBRARY_PATH="$LIBDIR" ./mcp3008.bin $1
