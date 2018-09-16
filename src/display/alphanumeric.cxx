#include <thread>
#include <chrono>
#include <exception/exception.hpp>
#include <display/alphanumeric.hpp>


#include <iostream>


namespace rasplib {
namespace display {




Alphanumeric::Alphanumeric()
{
    _num_columns = 0;
    _num_lines = 0;

    _chip = 0;
};




Alphanumeric::Alphanumeric(unsigned short columns, unsigned short lines)
{
    _num_columns = columns;
    _num_lines = lines;

    _chip = 0;
};




Alphanumeric::~Alphanumeric()
{
};



void Alphanumeric::Send(std::bitset<8> data, bool command)
{
    if(!_chip)
        throw rasplib::exception::Exception(201, "Cannot send data/command to display: unknown gpio device");

    _chip->Pin(_data_pins[0]).SetState(data[0]);
    _chip->Pin(_data_pins[1]).SetState(data[1]);
    _chip->Pin(_data_pins[2]).SetState(data[2]);
    _chip->Pin(_data_pins[3]).SetState(data[3]);
    _chip->Pin(_data_pins[4]).SetState(data[4]);
    _chip->Pin(_data_pins[5]).SetState(data[5]);
    _chip->Pin(_data_pins[6]).SetState(data[6]);
    _chip->Pin(_data_pins[7]).SetState(data[7]);
    _chip->Pin(_command_pin).SetState(!command);

    _chip->Pin(_send_pin).SetState(true);
    std::this_thread::sleep_for(std::chrono::nanoseconds(40));
    _chip->Pin(_send_pin).SetState(false);
}


void Alphanumeric::DisplayOn()
{
    if(!_chip)
        throw rasplib::exception::Exception(201, "Cannot turn on display: missing gpio chip reference");

    Send(std::bitset<8>(std::string("00001111")), true);
    Send(std::bitset<8>(std::string("00111100")), true);
};



void Alphanumeric::Init(rasplib::gpio::GPIOChip *chip,
                               unsigned short command_pin,
                               unsigned short send_pin,
                               std::vector<unsigned short> data_pins)
{
    _chip = chip;
    _command_pin = command_pin;
    _send_pin = send_pin;
    _data_pins = data_pins;

    DisplayOn();
    Clean();

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




void Alphanumeric::SetMode(unsigned short lines, bool cursor, bool blink)
{
    if(!_chip)
        throw rasplib::exception::Exception(201, "Cannot set number of lines: missing gpio chip reference");

    std::bitset<8> mode(std::string("00001000"));

    if(lines > 1)
        mode[2] = 1;

    if(cursor)
        mode[1] = 1;

    if(blink)
        mode[0] = 1;


    Send(mode, true);
};




void Alphanumeric::Clean()
{
    if(!_chip)
        throw rasplib::exception::Exception(201, "Cannot clear display: missing gpio chip reference");

    Send(std::bitset<8>(std::string("00000001")), true);
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
};





void Alphanumeric::Fill(unsigned short num)
{
    for(int i = 0; i < num; i++)
        Send(_map[' '], false);
};




void Alphanumeric::Print(std::string text)
{
    if(!_chip)
        throw rasplib::exception::Exception(302, "Cannot print text: missing gpio chip reference");

    int num_chars = 0;

    for(auto iter : text)
    {
        if(iter == '\n')
        {
            Fill(40 - num_chars);
            num_chars = 0;
            continue;
        }
        Send(_map[iter], false);
        num_chars++;
    }

    Fill(40 - num_chars);
};



}
}
