/*
    Rasplib - library for handling Raspberry Pi's GPIO and connected devices
    Copyright (C) 2018  Radosław Ulatowski

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/




#ifndef __RASPLIB_I2C_DEVICE_HPP__
#define __RASPLIB_I2C_DEVICE_HPP__




#include <bitset>




namespace rasplib {
namespace i2c {




class I2CDevice
{
private:
    int _handler;

public:
    I2CDevice();
    ~I2CDevice();

public:
    void open(unsigned short bus, std::bitset<8> address);
    void close();

public:
    void write(std::bitset<8> data);
    void read();
};




}
}

#endif
