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



#ifndef __RASPLIB_DISPLAY_ALPHANUMERIC_HPP__
#define __RASPLIB_DISPLAY_ALPHANUMERIC_HPP__

#include <vector>
#include <bitset>
#include <memory>
#include <map>
#include <gpio/device.hpp>
#include <i2c/device.hpp>



namespace rasplib {
namespace display {




class Alphanumeric
{
private:
    unsigned short _num_columns;
    unsigned short _num_lines;

private:
    rasplib::gpio::GPIODevice *_gpio_device;

    unsigned short _command_pin;
    unsigned short _send_pin;

    std::vector<unsigned short> _data_pins;

private:
    std::unique_ptr<rasplib::i2c::I2CDevice> _i2c_device;

private:
    std::map<char, std::bitset<8>> _map;
    void init_map();

private:
    void display_on();
    void fill(unsigned short num);

public:
    Alphanumeric();
    Alphanumeric(unsigned short columns, unsigned short lines);
    virtual ~Alphanumeric();

public:
    void init(rasplib::gpio::GPIODevice *gpio_device,
              unsigned short command_pin,
              unsigned short send_pin,
              std::vector<unsigned short> data_pins);

    void init(unsigned short bus, std::bitset<8> address);

    void set_mode(unsigned short lines, bool cursor, bool blink);

public:
    void send(std::bitset<8> data, bool command = false);

public:
    void clean();

    void print(std::string text);
};




}
}

#endif
