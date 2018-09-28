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




#include <thread>
#include <chrono>
#include <exception/exception.hpp>
#include <display/alphanumeric.hpp>




namespace rasplib {
namespace display {




Alphanumeric::Alphanumeric()
{
    _num_columns = 0;
    _num_lines = 0;

    _gpio_device = 0;
    _i2c_device = 0;
};




Alphanumeric::Alphanumeric(unsigned short columns, unsigned short lines)
{
    _num_columns = columns;
    _num_lines = lines;

    _gpio_device = 0;
    _i2c_device = 0;
};




Alphanumeric::~Alphanumeric()
{
    if(_i2c_device)
        _i2c_device->close();
};



void Alphanumeric::send(std::bitset<8> data, bool command)
{
    if(!_gpio_device && !_i2c_device)
        throw rasplib::Exception(201, "Cannot send data/command to display: missing device");

    if(_gpio_device)
    {
        _gpio_device->pin(_data_pins[0]).set_state(data[0]);
        _gpio_device->pin(_data_pins[1]).set_state(data[1]);
        _gpio_device->pin(_data_pins[2]).set_state(data[2]);
        _gpio_device->pin(_data_pins[3]).set_state(data[3]);
        _gpio_device->pin(_data_pins[4]).set_state(data[4]);
        _gpio_device->pin(_data_pins[5]).set_state(data[5]);
        _gpio_device->pin(_data_pins[6]).set_state(data[6]);
        _gpio_device->pin(_data_pins[7]).set_state(data[7]);
        _gpio_device->pin(_command_pin).set_state(!command);

        _gpio_device->pin(_send_pin).set_state(true);
        std::this_thread::sleep_for(std::chrono::nanoseconds(40));
        _gpio_device->pin(_send_pin).set_state(false);
    };

    if(_i2c_device)
    {
        if(command)
            _i2c_device->write(std::bitset<8>(0x40));
        else
            _i2c_device->write(std::bitset<8>(0x00));

        _i2c_device->write(data);
    };
}


void Alphanumeric::display_on()
{
    if(!_gpio_device && !_i2c_device)
        throw rasplib::Exception(201, "Cannot turn on display: missing device");

    send(std::bitset<8>(std::string("00001111")), true);
    send(std::bitset<8>(std::string("00111100")), true);
};



void Alphanumeric::init_map()
{
    _map[' '] = std::bitset<8>("00100000");

    _map['A'] = std::bitset<8>("01000001");
    _map['B'] = std::bitset<8>("01000010");
    _map['C'] = std::bitset<8>("01000011");
    _map['D'] = std::bitset<8>("01000100");
    _map['E'] = std::bitset<8>("01000101");
    _map['F'] = std::bitset<8>("01000110");
    _map['G'] = std::bitset<8>("01000111");
    _map['H'] = std::bitset<8>("01001000");
    _map['I'] = std::bitset<8>("01001001");
    _map['J'] = std::bitset<8>("01001010");
    _map['K'] = std::bitset<8>("01001011");
    _map['L'] = std::bitset<8>("01001100");
    _map['M'] = std::bitset<8>("01001101");
    _map['N'] = std::bitset<8>("01001110");
    _map['O'] = std::bitset<8>("01001111");
    _map['P'] = std::bitset<8>("01010000");
    _map['Q'] = std::bitset<8>("01010001");
    _map['R'] = std::bitset<8>("01010010");
    _map['S'] = std::bitset<8>("01010011");
    _map['T'] = std::bitset<8>("01010100");
    _map['U'] = std::bitset<8>("01010101");
    _map['V'] = std::bitset<8>("01010110");
    _map['W'] = std::bitset<8>("01010111");
    _map['X'] = std::bitset<8>("01011000");
    _map['Y'] = std::bitset<8>("01011001");
    _map['Z'] = std::bitset<8>("01011010");

    _map['a'] = std::bitset<8>("01100001");
    _map['b'] = std::bitset<8>("01100010");
    _map['c'] = std::bitset<8>("01100011");
    _map['d'] = std::bitset<8>("01100100");
    _map['e'] = std::bitset<8>("01100101");
    _map['f'] = std::bitset<8>("01100110");
    _map['g'] = std::bitset<8>("01100111");
    _map['h'] = std::bitset<8>("01101000");
    _map['i'] = std::bitset<8>("01101001");
    _map['j'] = std::bitset<8>("01101010");
    _map['k'] = std::bitset<8>("01101011");
    _map['l'] = std::bitset<8>("01101100");
    _map['m'] = std::bitset<8>("01101101");
    _map['n'] = std::bitset<8>("01101110");
    _map['o'] = std::bitset<8>("01101111");
    _map['p'] = std::bitset<8>("01110000");
    _map['q'] = std::bitset<8>("01110001");
    _map['r'] = std::bitset<8>("01110010");
    _map['s'] = std::bitset<8>("01110011");
    _map['t'] = std::bitset<8>("01110100");
    _map['u'] = std::bitset<8>("01110101");
    _map['v'] = std::bitset<8>("01110110");
    _map['w'] = std::bitset<8>("01110111");
    _map['x'] = std::bitset<8>("01111000");
    _map['y'] = std::bitset<8>("01111001");
    _map['z'] = std::bitset<8>("01111010");

    _map['0'] = std::bitset<8>("00110000");
    _map['1'] = std::bitset<8>("00110001");
    _map['2'] = std::bitset<8>("00110010");
    _map['3'] = std::bitset<8>("00110011");
    _map['4'] = std::bitset<8>("00110100");
    _map['5'] = std::bitset<8>("00110101");
    _map['6'] = std::bitset<8>("00110110");
    _map['7'] = std::bitset<8>("00110111");
    _map['8'] = std::bitset<8>("00111000");
    _map['9'] = std::bitset<8>("00111001");

    _map['!'] = std::bitset<8>("00100001");
    _map['"'] = std::bitset<8>("00100010");
    _map['#'] = std::bitset<8>("00100011");
    _map['$'] = std::bitset<8>("00100100");
    _map['%'] = std::bitset<8>("00100101");
    _map['&'] = std::bitset<8>("00100110");
    _map['\''] = std::bitset<8>("00100111");
    _map['('] = std::bitset<8>("00101000");
    _map[')'] = std::bitset<8>("00101001");
    _map['*'] = std::bitset<8>("00101010");
    _map['+'] = std::bitset<8>("00101011");
    _map[','] = std::bitset<8>("00101100");
    _map['-'] = std::bitset<8>("00101101");
    _map['.'] = std::bitset<8>("00101110");
    _map['/'] = std::bitset<8>("00101111");
    _map[':'] = std::bitset<8>("00111010");
    _map[';'] = std::bitset<8>("00111011");
    _map['<'] = std::bitset<8>("00111100");
    _map['='] = std::bitset<8>("00111101");
    _map['>'] = std::bitset<8>("00111110");
    _map['?'] = std::bitset<8>("00111111");

    _map['{'] = std::bitset<8>("01111011");
    _map['|'] = std::bitset<8>("01111100");
    _map['}'] = std::bitset<8>("01111101");
    _map['^'] = std::bitset<8>("01101110");
    _map['_'] = std::bitset<8>("01101111");
    _map['['] = std::bitset<8>("01011011");
    _map[']'] = std::bitset<8>("01011101");

    _map['`'] = std::bitset<8>("01100000");
};



void Alphanumeric::init(rasplib::gpio::GPIODevice *gpio_device,
                               unsigned short command_pin,
                               unsigned short send_pin,
                               std::vector<unsigned short> data_pins)
{
    _gpio_device = gpio_device;
    _command_pin = command_pin;
    _send_pin = send_pin;
    _data_pins = data_pins;

    display_on();
    clean();

    init_map();
};




void Alphanumeric::init(unsigned short bus, std::bitset<8> address)
{
    _i2c_device = std::make_unique<rasplib::i2c::I2CDevice>();
    _i2c_device->open(bus, address);

    display_on();
    clean();

    init_map();
};




void Alphanumeric::set_mode(unsigned short lines, bool cursor, bool blink)
{
    if(!_gpio_device && !_i2c_device)
        throw rasplib::Exception(201, "Cannot set number of lines: missing device");

    std::bitset<8> mode(std::string("00001000"));

    if(lines > 1)
        mode[2] = 1;

    if(cursor)
        mode[1] = 1;

    if(blink)
        mode[0] = 1;


    send(mode, true);
};




void Alphanumeric::clean()
{
    if(!_gpio_device && !_i2c_device)
        throw rasplib::Exception(201, "Cannot clear display: missing device");

    send(std::bitset<8>(std::string("00000001")), true);
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
};





void Alphanumeric::fill(unsigned short num)
{
    for(int i = 0; i < num; i++)
        send(_map[' '], false);
};




void Alphanumeric::print(std::string text)
{
    if(!_gpio_device && !_i2c_device)
        throw rasplib::Exception(302, "Cannot print text: missing device");

    int num_chars = 0;

    for(auto iter : text)
    {
        if(iter == '\n')
        {
            fill(40 - num_chars);
            num_chars = 0;
            continue;
        }
        send(_map[iter], false);
        num_chars++;
    }

    fill(40 - num_chars);
};



}
}
