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



#include <chrono>
#include <exception/exception.hpp>
#include <clock/clock.hpp>




namespace rasplib {
namespace clock {




Clock::Clock()
{
    _pin = 0;
    _frequency = 0;
    _stop = false;

    _before_handler_fun = 0;
    _before_handler_data = 0;

    _after_handler_fun = 0;
    _after_handler_data = 0;
};




Clock::~Clock()
{
    stop();
};




void Clock::clock_handler(Clock *self)
{
    self->handle();
};




void Clock::handle()
{
    unsigned long cycle = 1000000000 / _frequency;
    auto half_cycle = std::chrono::nanoseconds(cycle / 2);

    while(!_stop)
    {
        std::lock_guard<std::mutex> guard(_thread_mutex);

        if(_before_handler_fun)
            _before_handler_fun(this, _before_handler_data);

        _pin->set_state(1);
        std::this_thread::sleep_for(half_cycle);
        _pin->set_state(0);
        std::this_thread::sleep_for(half_cycle);

        if(_after_handler_fun)
            _after_handler_fun(this, _after_handler_data);
    };
};




void Clock::init(rasplib::gpio::GPIOPin *pin,  unsigned long frequency)
{
    if(frequency > 20000000)
        throw rasplib::Exception(FREQUENCY_NOT_SUPPORTED, "Frequency out of range");

    // set clock parameters
    _pin       = pin;
    _frequency = frequency;
};




void Clock::start()
{
    if(_clock_thread)
        return;

    _clock_thread = std::make_unique<std::thread>(clock_handler, this);
    _clock_thread->detach();
};




void Clock::stop()
{
    if(!_clock_thread)
        return;

    _stop = true;

    if(_clock_thread->joinable())
        _clock_thread->join();

    _stop = false;

    _clock_thread.reset();
};




void Clock::set_function(std::function<void(Clock *, void *)> fun, bool after, void *data)
{
    std::lock_guard<std::mutex> guard(_thread_mutex);

    if(after)
    {
        _after_handler_fun = fun;
        _after_handler_data = data;
    }
    else
    {
        _before_handler_fun = fun;
        _before_handler_data = data;
    };
};




void Clock::remove_function(bool after)
{
    std::lock_guard<std::mutex> guard(_thread_mutex);
    if(after)
    {
        _after_handler_fun = 0;
        _after_handler_data = 0;
    }
    else
    {
        _before_handler_fun = 0;
        _before_handler_data = 0;
    };
};

}
}
