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

//TODO: mcp3004 tests




#ifndef __RASPLIB_LIBRARY_MCP300X_HPP__
#define __RASPLIB_LIBRARY_MCP300X_HPP__




#include "spi/device.hpp"




namespace rasplib {
namespace library {




enum
{
    MCP_PORT_OUT_OF_RANGE = 1001
};




/**
 * Supported MCP300x chip types
 */
typedef enum
{
    MCP3004,
    MCP3008
} MCP300x_TYPE;




/**
 * MCP300x chip class
 */
class MCP300x
{
private:
    MCP300x_TYPE _type;                 //< MCP300x type
    rasplib::spi::SPIDevice *_device;   //< SPI device pointer

private:
    /**
     * Get maximum number of supported ports for device type
     *
     * @return Number of ports
     */
    unsigned short num_ports();

public:
    /**
     * Class contructor
     */
    MCP300x();

    /**
     * Class destructor
     */
    ~MCP300x();

public:
    /**
     * Set MCP300x type
     *
     * @param type MCP300x type
     */
    void set_type(MCP300x_TYPE type);

    /**
     * Get MCP300x type
     *
     * @return MCP300x type
     */
    MCP300x_TYPE get_type();

    /**
     * Set SPI device
     */
    void set_spi(rasplib::spi::SPIDevice *device);

public:
    /**
     * Read current value from device
     *
     * @param port Port to read value from (number of ports depend on device type)
     * @param differential Differential mode
     * @param extended_resolution Override / extend chip resolution on supported chips (in most cases by 1 bit)
     * @return Value
     */
    unsigned short read(unsigned short port, bool differential = false, bool extended_resolution = false);
};




}
}


#endif
