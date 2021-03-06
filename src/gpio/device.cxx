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




#include <sys/ioctl.h>
#include <linux/gpio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <exception/exception.hpp>
#include <gpio/device.hpp>


namespace rasplib {
namespace gpio {




GPIODevice::GPIODevice()
{
    _handler = 0;
    _lines = 0;
};



GPIODevice::~GPIODevice()
{
    // close device if opened
    if(is_open())
        close();
};




std::string GPIODevice::name()
{
    return _name;
};




std::string GPIODevice::label()
{
    return _label;
};



unsigned short GPIODevice::lines()
{
    return _lines;
};




void GPIODevice::open(std::string device)
{
    // close device if is opened
    if(is_open())
        close();

    try
    {
        // try to open system device
        _handler = ::open(device.c_str(), O_RDWR | O_CLOEXEC);
        if(_handler < 0)
            throw rasplib::Exception(INVALID_DEVICE, std::string("Cannot open device: ") + device);

        struct gpiochip_info info;
        std::memset(&info, 0, sizeof(info));

        // get device info
        auto status = ioctl(_handler, GPIO_GET_CHIPINFO_IOCTL, &info);
        if(status < 0)
            throw rasplib::Exception(DEVICE_INFO_ERROR, "Unable to get gpio device info");

        // copy device info
        _name = info.name;
        _lines = info.lines;
        _label = info.label;

        // create pins objects
        for(int line = 0; line < _lines; line++)
            _pins.push_back(std::make_shared<rasplib::gpio::GPIOPin>(this, line));
    }
    catch(rasplib::Exception e)
    {
        // clean when something went wrong
        if(_handler)
        {
            ::close(_handler);
            _handler = 0;
        };

        throw e;
    };
};



void GPIODevice::close()
{
    // if handler is not 0 close device
    if(_handler)
    {
        ::close(_handler);
        _handler = 0;
    };
};




bool GPIODevice::is_open()
{
    // if handler is not 0 then device is opened
    if(_handler)
        return true;

    return false;
};



int GPIODevice::handler()
{
    return _handler;
};



GPIOPin& GPIODevice::pin(unsigned short num)
{
    // check if requested pin number is valid
    if(num >= _pins.size())
        throw rasplib::Exception(INVALID_PIN_REQUEST, "Requested line number is higher than available lines");

    // return pin object
    return *_pins[num];
};


}
}
