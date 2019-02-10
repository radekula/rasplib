#!/bin/bash

make
LD_LIBRARY_PATH=/home/radek/Projekty/rasplib/build ./mcp3008.bin
