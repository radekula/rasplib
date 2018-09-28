#!/bin/bash

make
LD_LIBRARY_PATH=/home/radek/Projekty/rasplib/build ./lcd_i2c.bin
