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



#ifndef __RASPLIB_CLOCK_CLOCK_HPP__
#define __RASPLIB_CLOCK_CLOCK_HPP__

#include <vector>
#include <bitset>
#include <memory>
#include <map>
#include <thread>
#include <mutex>
#include <gpio/pin.hpp>



namespace rasplib {
namespace clock {




class Clock
{
private:
    rasplib::gpio::GPIOPin *_pin;               //< Pointer to GPIO pin where clock will be outputted
    unsigned long _frequency;

private:
    std::unique_ptr<std::thread> _clock_thread; //< Clock thread pointer
    bool _stop;                                 //< Stop clock thread when true

private:
    /**
     * Thread function for handling clock function
     *
     * @param self Pointer to Clock object
     */
    static void clock_handler(Clock *self);

    /**
     * Main function to handle clock
     */
    void handle();

public:
    /**
     * Default constructor
     */
    Clock();

    /**
     * Class destructor
     */
    virtual ~Clock();

public:
    /**
     * Init class to work with GPIO pin
     *
     * @param pin Pointer to GPIO pin
     * @param frequency Clock frequency
     */
    void init(rasplib::gpio::GPIOPin *pin,
              unsigned long frequency);

public:
    /**
     * Start clock
     */
    void start();

    /**
     * Stop clock
     */
    void stop();

};




}
}

#endif
