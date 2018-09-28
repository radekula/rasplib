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
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <exception/exception.hpp>
#include <i2c/device.hpp>



namespace rasplib {
namespace i2c {




I2CDevice::I2CDevice()
{
    _handler = 0;
};




I2CDevice::~I2CDevice()
{
    close();
};




void I2CDevice::open(unsigned short bus, std::bitset<8> address)
{
    std::string dev("/dev/i2c-");
    dev.append(std::to_string(bus));

    _handler = ::open(dev.c_str(), O_RDWR);
    if(_handler <= 0)
        throw rasplib::Exception(I2C_BUS_ERROR, "Unable to get I2C bus device");

    auto res = ioctl(_handler, I2C_SLAVE, (uint8_t) address.to_ulong());
    if(res < 0)
    {
        ::close(_handler);
        throw rasplib::Exception(I2C_DEVICE_ERROR, "Unable to get I2C device");
    };
};




void I2CDevice::close()
{
    if(_handler)
        ::close(_handler);

    _handler = 0;
};




void I2CDevice::write(std::bitset<8> data)
{
    if(_handler <= 0)
        throw rasplib::Exception(I2C_DEVICE_ERROR, "Device not opened");

    uint8_t _data = (uint8_t) data.to_ulong();
    ::write(_handler, &_data, sizeof(_data));
};




void I2CDevice::read()
{
};




}
}
