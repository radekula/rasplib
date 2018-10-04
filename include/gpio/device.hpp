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
    int _handler;                                   //< System handler id for device (file handler)

private:
    std::string _name;                              //< Name of a device
    std::string _label;                             //< Device label
    unsigned short _lines;                          //< Number of device's GPIO lines

private:
    std::vector<std::shared_ptr<GPIOPin>> _pins;    //< Array of pins objects

public:
    /**
     * Class constructor
     */
    GPIODevice();

    /**
     * Class destructor
     */
    ~GPIODevice();

public:
    /**
     * Get name of a device
     */
    std::string name();

    /**
     * Get device label
     */
    std::string label();

    /**
     * Get number of GPIO lines
     */
    unsigned short lines();

public:
    /**
     * Open a GPIO device
     *
     * @param device Device path (/dev/gpiochipN)
     */
    void open(std::string device);

    /**
     * Close a device
     */
    void close();

    /**
     * Check if device is opened
     *
     * @return True when device is opened otherwise false
     */
    bool is_open();

    /**
     * Get device handler id
     *
     * @return Device handler
     */
    int handler();

public:
    /**
     * Get access to a device pin
     *
     * @param num Pin number
     * @return Pin object
     */
    GPIOPin& pin(unsigned short num);
};



}
}

#endif
