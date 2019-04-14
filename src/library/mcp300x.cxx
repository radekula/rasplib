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




#include "library/mcp300x.hpp"
#include <exception/exception.hpp>




namespace rasplib {
namespace library {




MCP300x::MCP300x()
{
    _type = MCP3008;

    _device = 0;
}




MCP300x::~MCP300x()
{
};




void MCP300x::set_type(MCP300x_TYPE type)
{
    _type = type;
};




MCP300x_TYPE MCP300x::get_type()
{
    return _type;
};




void MCP300x::set_spi(rasplib::spi::SPIDevice *device)
{
    _device = device;
};




unsigned short MCP300x::read(unsigned short port, bool differential, bool extended_resolution)
{
    unsigned short value = 0;

    if(port >= num_ports())
        throw rasplib::Exception(MCP_PORT_OUT_OF_RANGE, "Number of port is out of range for device.");

    std::vector<bool> to_send;

    // communication starts with first
    to_send.push_back(0);
    to_send.push_back(1);

    // setup device
    to_send.push_back(!differential);
    to_send.push_back(port & 1);
    to_send.push_back(port & 2);
    to_send.push_back(port & 4);

    // prepare extra bits (waiting for data)
    for(short i = 0; i < 32; i++)
        to_send.push_back(0);

    // Enable device by setting state on cs pin
    _device->set_state_cs(1);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    _device->set_state_cs(0);

    auto ret = _device->send(to_send);

    for(auto val : ret)
    {
        value = val;
        value << 1;
    };

    return value;
};




}
}
