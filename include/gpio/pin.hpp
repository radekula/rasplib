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




#ifndef __RASPLIB_GPIO_PIN_HPP__
#define __RASPLIB_GPIO_PIN_HPP__




#include <memory>
#include <functional>
#include <linux/gpio.h>




namespace rasplib {
namespace gpio {



class GPIODevice;



/**
 * Enum type for types of signal edges
 */
typedef enum {
    RISING = 1,
    FALLING
} Edge;





class GPIOPin
{
private:
    GPIODevice *_device;                                                //< Pointer to parent GPIO device
    short _line;                                                        //< Memorized line numeber paired with pin object
    gpiohandle_request _req;                                            //< Request object for communicating with system device

private:
    std::function<void(GPIOPin*, int)> _handle_fun;                     //< Function object to external function for handling changes in pin (used in interrupts)

    /**
     * Get request handler for a pin
     *
     * @param type System id for request type (GPIOHANDLE_REQUEST_OUTPUT / GPIOHANDLE_REQUEST_INPUT)
     * @return System handler id
     */
    int request(int type);

    /**
     * Free request handler
     */
    void close_request();

public:
    /**
     * Class default constructor
     */
    GPIOPin();

    /**
     * Class contructor
     *
     * @param gpio_device Pointer to parent GPIO device
     * @param line Line number paired with pin
     */
    GPIOPin(GPIODevice *gpio_device, unsigned short line);

    /**
     * Class destructor
     */
    ~GPIOPin();

public:
    /**
     * Set parent GPIO device
     *
     * @param gpio_device Pointer to parent GPIO device
     */
    void set_device(GPIODevice *gpio_device);

    /**
     * Get parent device pointer
     *
     * @return Pointer to parent device
     */
    GPIODevice* device();

public:
    /**
     * Pair pin with given device line
     *
     * @param line Device line number
     */
    void map_to_line(unsigned short line);

    /**
     * Set external handler function for interrupts
     *
     * @param fun External function object
     */
    void set_function(std::function<void(GPIOPin*, int)> fun);

public:
    /**
     * Set pin / line state
     *
     * @param state New state
     */
    void set_state(bool state);

    /**
     * Get current pin / line state
     *
     * @return Current pin / lane state
     */
    bool state();

    /**
     * Get line number paired with pin
     *
     * @return Line number
     */
    short line();

public:
    /**
     * Function to handle interrups.
     *
     * If there is external function object associated with pin this function will call that funciton.
     */
    void handle_interrupt(int edge);
};




}
}

#endif
