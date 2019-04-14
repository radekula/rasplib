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




#include <exception/exception.hpp>
#include <spi/device.hpp>




namespace rasplib {
namespace spi {




SPIDevice::SPIDevice()
{
    // default mode: software
    _hardware = false;

    _cs    = 0;
    _d_in  = 0;
    _d_out = 0;
    _clock = 0;
};




SPIDevice::~SPIDevice()
{
};




void SPIDevice::init(rasplib::gpio::GPIOPin *cs,
                     rasplib::gpio::GPIOPin *d_in,
                     rasplib::gpio::GPIOPin *d_out,
                     rasplib::clock::Clock *clock)
{
    _hardware = false;

    _cs    = cs;
    _d_in  = d_in;
    _d_out = d_out;
    _clock = clock;
};





void SPIDevice::clock_handler(rasplib::clock::Clock *Clock, void *data)
{
    ((SPIDevice *) data)->handle();
};




void SPIDevice::handle()
{
    if(_cycle >= _send.size())
    {
        _done = true;
        return;
    };

    _d_in->set_state(_send[_cycle]);
    _received.push_back((bool) _d_out->state());

    _cycle++;
}




void SPIDevice::set_state_cs(bool state)
{
    _cs->set_state((int) state);
};




std::vector<bool> SPIDevice::send(std::vector<bool> data)
{
    // prepare
    _cycle = 0;
    _done = false;
    _send = data;
    _received.clear();

    // register function in clock object
    _clock->set_function(clock_handler, false, (void *) this);

    // wait for reading to finish
    while(!_done)
        std::this_thread::sleep_for(std::chrono::nanoseconds(100));

    // unregister function
    _clock->remove_function(false);

    return _received;
}




}
}
