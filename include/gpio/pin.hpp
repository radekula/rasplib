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




#ifndef __RASPLIB_GPIO_PIN_HPP__
#define __RASPLIB_GPIO_PIN_HPP__




#include <memory>
#include <functional>
#include <linux/gpio.h>




namespace rasplib {
namespace gpio {


class GPIODevice;



typedef enum {
    RISING = 1,
    FALLING
} Edge;





class GPIOPin
{
private:
    GPIODevice *_device;
    short _line;

private:
    std::function<void(GPIOPin*, int)> _handle_fun;

public:
    GPIOPin();
    GPIOPin(GPIODevice *gpio_device, unsigned short line);
    ~GPIOPin();

public:
    void set_device(GPIODevice *gpio_device);
    GPIODevice* get_device();

public:
    void map_to_line(unsigned short line);
    void set_function(std::function<void(GPIOPin*, int)> fun);

public:
    void set_state(bool state);
    bool get_state();
    short get_line();

public:
    void handle_interrupt(int edge);
};




}
}

#endif
