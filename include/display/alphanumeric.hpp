#ifndef __RASPLIB_DISPLAY_ALPHANUMERIC_HPP__
#define __RASPLIB_DISPLAY_ALPHANUMERIC_HPP__

#include <vector>
#include <bitset>
#include <memory>
#include <map>
#include <gpio/gpio_chip.hpp>


namespace rasplib {
namespace display {




class Alphanumeric
{
private:
    unsigned short _num_columns;
    unsigned short _num_lines;

private:
    rasplib::gpio::GPIOChip *_chip;

    unsigned short _command_pin;
    unsigned short _send_pin;

    std::vector<unsigned short> _data_pins;

private:
    std::map<char, std::bitset<8>> _map;

private:
    void DisplayOn();
    void Fill(unsigned short num);

public:
    Alphanumeric();
    Alphanumeric(unsigned short columns, unsigned short lines);
    virtual ~Alphanumeric();

public:
    void Init(rasplib::gpio::GPIOChip *chip,
              unsigned short command_pin,
              unsigned short send_pin,
              std::vector<unsigned short> data_pins);

    void SetMode(unsigned short lines, bool cursor, bool blink);

public:
    void Send(std::bitset<8> data, bool command = false);

public:
    void Clean();

    void Print(std::string text);
};




}
}

#endif
