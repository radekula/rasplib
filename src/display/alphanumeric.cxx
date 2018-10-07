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
#include <sstream>
#include <exception/exception.hpp>
#include <display/alphanumeric.hpp>




namespace rasplib {
namespace display {




Alphanumeric::Alphanumeric()
{
    _num_columns = 0;
    _num_lines = 0;
    _virtual_line_length = 40;

    _full_screen = false;
    _backlight = true;
    _cursor_visible = false;
    _cursor_blink = false;

    _gpio_device = 0;
    _i2c_device = 0;

    _command_pin = 0;
    _send_pin = 0;
};




Alphanumeric::Alphanumeric(unsigned short columns, unsigned short lines, unsigned short virtual_line_length)
{
    _num_columns = columns;
    _num_lines = lines;
    _virtual_line_length = virtual_line_length;

    _full_screen = false;
    _backlight = true;
    _cursor_visible = false;
    _cursor_blink = false;

    _gpio_device = 0;
    _i2c_device = 0;

    _command_pin = 0;
    _send_pin = 0;
};




Alphanumeric::~Alphanumeric()
{
    if(_i2c_device)
        _i2c_device->close();
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




void Alphanumeric::send(std::bitset<8> data, bool command)
{
    if(!_gpio_device && !_i2c_device)
        throw rasplib::Exception(201, "Cannot send data/command to display: missing device");

    // With small code modification it may be possibile 
    // to send data to GPIO and I2C devices at the same time
    // but most use cases don't need this feature
    if(_gpio_device)
    {
        // set data pins values
        _gpio_device->pin(_data_pins[0]).set_state(data[0]);
        _gpio_device->pin(_data_pins[1]).set_state(data[1]);
        _gpio_device->pin(_data_pins[2]).set_state(data[2]);
        _gpio_device->pin(_data_pins[3]).set_state(data[3]);
        _gpio_device->pin(_data_pins[4]).set_state(data[4]);
        _gpio_device->pin(_data_pins[5]).set_state(data[5]);
        _gpio_device->pin(_data_pins[6]).set_state(data[6]);
        _gpio_device->pin(_data_pins[7]).set_state(data[7]);

        // set command pin value
        _gpio_device->pin(_command_pin).set_state(!command);

        // set send pin to high state
        _gpio_device->pin(_send_pin).set_state(true);

        // most operations need around 40 us for command to execute
        // extra time for other commands is handled in places of sending
        std::this_thread::sleep_for(std::chrono::nanoseconds(40));

        // reset send pin to low state
        _gpio_device->pin(_send_pin).set_state(false);

    } else if(_i2c_device)
    {
        std::bitset<8> s(0x0);

        // bit 0 is data indicator (opposite to command)
        s[0] = !command;

        // bit 3 is backlight (need to be send with each command)
        s[3] = _backlight;

        // first we send most significant bits
        s[4] = data[4];
        s[5] = data[5];
        s[6] = data[6];
        s[7] = data[7];

        // write data to I2C device
        _i2c_device->write(s);

        // set execute bit to high and send (bit 2)
        // we cannot send one bit so we need to send whole byte again
        s[2] = 1;
        _i2c_device->write(s);

        // wait for command to execute (see send to GPIO device to get more information)
        std::this_thread::sleep_for(std::chrono::nanoseconds(50));

        // set execute bit to low and send
        s[2] = 0;
        _i2c_device->write(s);

        // repeat for least significant bits
        s[4] = data[0];
        s[5] = data[1];
        s[6] = data[2];
        s[7] = data[3];

        _i2c_device->write(s);

        s[2] = 1;
        _i2c_device->write(s);

        std::this_thread::sleep_for(std::chrono::nanoseconds(50));

        s[2] = 0;
        _i2c_device->write(s);
    };
}




//TODO This function works but this is not a proper initialization sequence - need fixing
void Alphanumeric::screen_on()
{
    if(!_gpio_device && !_i2c_device)
        throw rasplib::Exception(201, "Cannot turn on display: missing device");

    if(_i2c_device)
    {
        // send reset command (directly)
        _i2c_device->write(0b00001000);
        _i2c_device->write(0b00001100);                             // flip execute bit
        std::this_thread::sleep_for(std::chrono::milliseconds(1));  // time needs tunning
        _i2c_device->write(0b00001000);

        // set 4bit mode (directly)
        _i2c_device->write(0b00101000);
        _i2c_device->write(0b00101100);                            // flip execute bit
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // time needs tunning
        _i2c_device->write(0b00101000);
    }

    // send default mode to screen
    send(std::bitset<8>(std::string("00001111")), true);
};




void Alphanumeric::fill(unsigned short num)
{
    // in loop send empty characters (spaces) to screen
    for(int i = 0; i < num; i++)
        send(_map[' '], false);
};




void Alphanumeric::init(rasplib::gpio::GPIODevice *gpio_device,
                               unsigned short command_pin,
                               unsigned short send_pin,
                               std::vector<unsigned short> data_pins)
{
    // set GPIO parameters
    _gpio_device = gpio_device;
    _command_pin = command_pin;
    _send_pin = send_pin;
    _data_pins = data_pins;

    // turn screen on
    screen_on();

    // always clean a screen
    clean();

    // init character map
    init_map();
};




void Alphanumeric::init(unsigned short bus, std::bitset<8> address)
{
    // open I2C device
    _i2c_device = std::make_unique<rasplib::i2c::I2CDevice>();
    _i2c_device->open(bus, address);

    // turn screen on
    screen_on();

    // always clean a screen
    clean();

    // init character map
    init_map();
};




void Alphanumeric::set_mode(bool full_screen, bool cursor, bool blink)
{
    if(!_gpio_device && !_i2c_device)
        throw rasplib::Exception(201, "Cannot set number of lines: missing device");

    // remember new settings
    _full_screen = full_screen;
    _cursor_visible = cursor;
    _cursor_blink = blink;

    std::bitset<8> func(0x0);

    // not important - may be 0 or 1
    func[0] = 0;
    // not important - may be 0 or 1
    func[1] = 0;
    // font size
    func[2] = 0;
    // set full_screen
    func[3] = full_screen;
    // 4/8 bit mode
    func[4] = _i2c_device ? 0 : 1;
    // always 1 (function set)
    func[5] = 1;
    // send new function set
    send(func, true);

    std::bitset<8> disp(0x0);

    // blinking cursor
    disp[0] = blink;
    // cursor visibility
    disp[1] = cursor;
    // screen on (always 1)
    disp[2] = 1;
    // always 1 (display switch)
    disp[3] = 1;
    // set new display parameters
    send(disp, true);
};




void Alphanumeric::set_physical(unsigned short columns, unsigned short lines, unsigned short virtual_line_length)
{
    _num_columns = columns;
    _num_lines = lines;
    _virtual_line_length = virtual_line_length;
};




void Alphanumeric::clean()
{
    if(!_gpio_device && !_i2c_device)
        throw rasplib::Exception(201, "Cannot clear display: missing device");

    // send display clean command
    send(std::bitset<8>(0x1), true);
    _current_text="";

    // wait for a longer time for command to execute (min. 1.8 milliseconds for some displays)
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
};




void Alphanumeric::print(std::string text)
{
    //TODO this should as parameters not constant
    int lines_addr[] = { 0x00, 0x40, 0x14, 0x54 };

    if(!_gpio_device && !_i2c_device)
        throw rasplib::Exception(302, "Cannot print text: missing device");

    // remember current line number
    int line = 0;

    // set cursor to begin
    std::bitset<8> start_addr(0x80 | lines_addr[line]);
    send(start_addr, true);

    std::istringstream new_text(text);
    std::istringstream old_text(_current_text);

    std::string new_line;
    std::string old_line;

    // compare each new line with old line and update if change is detected
    while(std::getline(new_text, new_line))
    {
        std::getline(old_text, old_line);
        auto old_size = old_line.size();

        int curr_char = -1;
        for(auto iter : new_line)
        {
            curr_char++;
            if(curr_char >= _virtual_line_length)
                break;

            if(curr_char < old_size)
            {
                if(new_line[curr_char] == old_line[curr_char])
                    continue;
            }

            // set cursor to place where change is needed
            std::bitset<8> start_addr((0x80 | lines_addr[line]) + curr_char);
            send(start_addr, true);
            send(_map[iter], false);
        };

        // set cursor to end of line
        std::bitset<8> start_addr((0x80 | lines_addr[line]) + ++curr_char);
        send(start_addr, true);

        // fill line with empty characters
        fill(_virtual_line_length - curr_char);

        // increment line number or reset to beginning
        line++;
        if(line == _num_lines)
            line = 0;

        // set cursor to new line
        std::bitset<8> line_addr(0x80 | lines_addr[line]);
        send(line_addr, true);
    };

    _current_text = text;
};



void Alphanumeric::backlight(bool enable)
{
    _backlight = enable;
};



}
}
