#!/bin/bash

make
echo "Running"
LD_LIBRARY_PATH=/home/radek/Projekty/rasplib/build ./lcd_i2c_4lines.bin
