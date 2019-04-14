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

//TODO: hardware mode




#ifndef __RASPLIB_SPI_DEVICE_HPP__
#define __RASPLIB_SPI_DEVICE_HPP__




#include <vector>
#include "gpio/device.hpp"
#include "clock/clock.hpp"




namespace rasplib {
namespace spi {




class SPIDevice
{
private:
    bool _hardware;                                                     //< Define if device is software (false) or hardware (true) device

private:
    // Software device variables
    rasplib::gpio::GPIOPin *_cs;                                        //< Device select pin
    rasplib::gpio::GPIOPin *_d_in;                                      //< Data in pin (connected to device in pin)
    rasplib::gpio::GPIOPin *_d_out;                                     //< Data out pin (connected to device out pin)
    rasplib::clock::Clock *_clock;                                      //< Clock pin

private:
    unsigned short _cycle;                                              //< Clock cycle number
    bool _done;                                                         //< Indicates if transfer is done

    std::vector<bool> _send;                                           //< Data to send
    std::vector<bool> _received;                                        //< Received data

private:
    /**
     * Handle clock callbacks
     *
     * @param Clock Pointer to clock that called function
     * @param data Pointer to custom data (pointer to self)
     */
    static void clock_handler(rasplib::clock::Clock *Clock, void *data);

    /**
     * Handle data exchange on each clock tick
     */
    void handle();

public:
    /**
     * Contructor for unspecified device
     */
    SPIDevice();

    /**
     * Class destructor
     */
    ~SPIDevice();

public:
    /**
     * Init device in software mode
     *
     * @param cs Device select pin pointer
     * @param d_in Data in pin pointer
     * @param d_out Data out pin pointer
     * @param clock Clock pin pointer
     */
    void init(rasplib::gpio::GPIOPin *cs,
              rasplib::gpio::GPIOPin *d_in,
              rasplib::gpio::GPIOPin *d_out,
              rasplib::clock::Clock *clock);

public:
    /**
     * Set state on cable select pin
     *
     * @param state State to set
     */
    void set_state_cs(bool state);

    /**
     * Send/receive data from device
     *
     * @param data Data to send
     * @return Data received
     */
    std::vector<bool> send(std::vector<bool> data);
};




}
}

#endif
