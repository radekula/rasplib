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



#ifndef __RASPLIB_DISPLAY_ALPHANUMERIC_HPP__
#define __RASPLIB_DISPLAY_ALPHANUMERIC_HPP__

#include <vector>
#include <bitset>
#include <memory>
#include <map>
#include <gpio/device.hpp>
#include <i2c/device.hpp>



namespace rasplib {
namespace display {




class Alphanumeric
{
private:
    unsigned short _num_columns;                           //< Physical screen dimensions: number of columns
    unsigned short _num_lines;                             //< Physical screen dimensions: number of rows
    unsigned short _virtual_line_length;                   //< Length of a virtual line (length of screen line buffer)

private:
    bool _full_screen;                                     //< Control usable screen size (false - half screen; true - full screen)
    bool _backlight;                                       //< Control screen backlight (false - off; true - on)
    bool _cursor_visible;                                  //< Control visibility of cursor (false - cursor not visible; true - cursor visible)
    bool _cursor_blink;                                    //< Control cursor blinking (false - no blink, true - blink)

private:
    std::string _current_text;                             //< Current displayed text

private:
    rasplib::gpio::GPIODevice *_gpio_device;               //< Pointer to GPIO device (if screen is connected via GPIO) [ONLY 8-bit mode is supported for now]
    std::unique_ptr<rasplib::i2c::I2CDevice> _i2c_device;  //< Pointer to I2C device (if screen is connected via I2C bus) [ONLY 4-bit mode is suppported for now]

private:
    unsigned short _command_pin;                           //< Command/Data pin number (GPIO only)
    unsigned short _send_pin;                              //< Send (execute) pin number (GPIO only)

    std::vector<unsigned short> _data_pins;                //< Array with pins numbers for data (GPIO only)

private:
    std::map<char, std::bitset<8>> _map;                   //< Character mapping

private:
    /**
     * Init character map with default values
     */
    void init_map();

    /**
     * Send data to screen
     *
     * @param data Binary data to send
     * @param command Command/Data switch
     */
    void send(std::bitset<8> data, bool command = false);

private:
    /**
     * Turn screen on (init screen on hardware level)
     */
    void screen_on();

    /**
     * Print empty characters (spaces) on screen
     *
     * @param num Number of empty characters to fill
     */
    void fill(unsigned short num);

public:
    /**
     * Default constructor
     */
    Alphanumeric();

    /**
     * Contruct new object with initial values
     *
     * @param columns Number of physical columns
     * @param lines Number of physical lines
     * @param virtual_line_lenght Length of virtual line (length of buffer)
     */
    Alphanumeric(unsigned short columns = 16,
                 unsigned short lines = 2,
                 unsigned short virtual_line_lenght = 40);

    /**
     * Class destructor
     */
    virtual ~Alphanumeric();

public:
    /**
     * Init class to work with GPIO
     *
     * @param gpio_device Pointer to GPIO device
     * @param command_pin Command/Data pin number
     * @param send_pin Send (execute) pin number
     * @param data_pins Array with pins numbers for data transfer
     */
    void init(rasplib::gpio::GPIODevice *gpio_device,
              unsigned short command_pin,
              unsigned short send_pin,
              std::vector<unsigned short> data_pins);

    /**
     * Init class to work with I2C bus
     *
     * @param bus I2C Bus number (/dev/i2c-{bus})
     * @param address Device addres on I2C bus
     */
    void init(unsigned short bus, std::bitset<8> address);

    /**
     * Set screen work mode
     *
     * @param full_screen Set half/full screen mode
     * @param cursor Set cursor visibility
     * @param blink Set cursor blinking
     */
    void set_mode(bool full_screen, bool cursor, bool blink);

    /**
     * Set screen physical parameters
     *
     * @param columns Set number of columns on screen
     * @param lines Set number of lines on screen
     * @param virtual_line_lenght Set virtual line length (buffer lenght)
     */
    void set_physical(unsigned short columns = 16, unsigned short lines = 2, unsigned short virtual_line_lenght = 40);

public:
    /**
     * Clean screen
     */
    void clean();

    /**
     * Print text on a screen
     */
    void print(std::string text);
};




}
}

#endif
