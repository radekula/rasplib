# rasplib

*** Description ***
This is a small, custom library to handle Raspberry's I/O ports (and maybe other devices too) written in C++.
This library uses new kernel GPIO interface (/dev/gpiochipN character device) so modern kernel is needed.

Tested on:
 * Fedora 28
 * Raspberry Pi 3 B+

As for now this is more a 'proof of concept' project or 'work in progress' project - not ready for production systems!.

*** Modules/APIs ***
 1. GPIO
 2. I2C
 3. Display
   * Alphanumeric LCDs (HD44780 compatible, PCF8574 I2C Bus Expander support)

*** Usage ***
See examples and raspscreen project.

*** TODO ***
 * [ALL] modern C++ where possible
 * [ALL] cleaning + comments
 * [I2C] read data from I2C device
 * [Alphanumeric] 4-bit GPIO mode
 * [Alphanumeric] 8-bit I2C mode (really needed?)
 * [Alphanumeric] support for more characters
 * [Alphanumeric] custom characters
 * [Alphanumeric] cursor moving to position
 * [Alphanumeric] timings as variables (different to each display)
 * [Alphanumeric] reading display parameters from file/config/pre-configs
