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
#include <unistd.h>
#include <linux/gpio.h>
#include <string>
#include <cstring>
#include <exception/exception.hpp>
#include <gpio/gpio_device.hpp>



namespace rasplib {
namespace gpio {




GPIOPin::GPIOPin()
{
    _line = -1;
    _gpio_device = 0;
};




GPIOPin::GPIOPin(GPIODevice *gpio_device, unsigned short line)
{
    _line = -1;
    _gpio_device = 0;

    set_gpio(gpio_device);
    map_to_line(line);
};




GPIOPin::~GPIOPin()
{
    release();
};




void GPIOPin::set_gpio(GPIODevice *device)
{
    _gpio_device = device;
};



void GPIOPin::map_to_line(unsigned short line)
{
    if(_line >= 0)
        release();

    _line = (short) line;
};




void GPIOPin::set_state(bool state)
{
    if(!_gpio_device)
        throw rasplib::Exception(INVALID_DEVICE, "Device reference is not available");

    release();

    struct gpiohandle_data data;
    std::memset(&data, 0, sizeof(data));

    _req.lineoffsets[0] = _line;
    _req.lines = 1;
    _req.flags = GPIOHANDLE_REQUEST_OUTPUT;

    int status = 0;

    status = ioctl(_gpio_device->get_handler(), GPIO_GET_LINEHANDLE_IOCTL, &_req);
    if(status < 0)
        throw rasplib::Exception(REQUEST_ERROR, "Unable to get gpio request");

    data.values[0] = state;

    status = ioctl(_req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
    if(status < 0)
        throw rasplib::Exception(LINE_WRITE_ERROR, "Unable to write new line state to gpio device");
};




bool GPIOPin::get_state()
{
    if(!_gpio_device)
        throw rasplib::Exception(INVALID_DEVICE, "Device reference is not available");

    release();

    struct gpiohandle_data data;
    std::memset(&data, 0, sizeof(data));

    _req.lineoffsets[0] = _line;
    _req.lines = 1;
    _req.flags = GPIOHANDLE_REQUEST_OUTPUT;

    int status = 0;

    status = ioctl(_gpio_device->get_handler(), GPIO_GET_LINEHANDLE_IOCTL, &_req);
    if(status < 0)
        throw rasplib::Exception(REQUEST_ERROR, "Unable to get gpio request");

    status = ioctl(_req.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data);
    if(status < 0)
        throw rasplib::Exception(LINE_WRITE_ERROR, "Unable to write new line state to gpio device");

    return data.values[0];
};



void GPIOPin::release()
{
    if(_req.fd)
        close(_req.fd);

    std::memset(&_req, 0, sizeof(_req));
};

}
}
