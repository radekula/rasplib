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




#ifndef __RASPLIB_GPIO_DEVICE_HPP__
#define __RASPLIB_GPIO_DEVICE_HPP__


#include <string>
#include <vector>
#include <memory>
#include <gpio/pin.hpp>



namespace rasplib {
namespace gpio {



class GPIODevice 
{
private:
    int _handler;

private:
    std::string _name;
    std::string _label;
    unsigned short _lines;

private:
    std::vector<std::shared_ptr<GPIOPin>> _pins;

public:
    GPIODevice();
    ~GPIODevice();

public:
    std::string name();
    std::string label();
    unsigned short lines();

public:
    void open(std::string device);
    void close();
    bool is_open();

    int get_handler();

public:
    GPIOPin& pin(unsigned short num);
};



}
}

#endif
