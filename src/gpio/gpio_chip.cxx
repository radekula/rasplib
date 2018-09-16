#include <sys/ioctl.h>
#include <linux/gpio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory.h>
#include <exception/exception.hpp>
#include <gpio/gpio_chip.hpp>


namespace rasplib {
namespace gpio {




GPIOChip::GPIOChip()
{
    _handler = 0;
    _lines = 0;
};



GPIOChip::~GPIOChip()
{
    if(IsOpen())
        Close();
};




std::string GPIOChip::Name()
{
    return std::string(_name);
};




std::string GPIOChip::Label()
{
    return std::string(_label);
};



unsigned short GPIOChip::Lines()
{
    return _lines;
};




void GPIOChip::Open(std::string device)
{
    if(IsOpen())
        Close();

    try
    {
        _handler = open(device.c_str(), O_RDWR | O_CLOEXEC);
        if(_handler < 0)
            throw rasplib::exception::Exception(1, std::string("Cannot open device: ") + device);

        struct gpiochip_info info;
        memset(&info, 0, sizeof(info));

        auto status = ioctl(_handler, GPIO_GET_CHIPINFO_IOCTL, &info);
        if(status < 0)
            throw rasplib::exception::Exception(2, "Unable to get gpio device info");

        _name = info.name;
        _lines = info.lines;
        _label = info.label;

        for(int line = 0; line < _lines; line++)
            _pins.push_back(std::make_shared<rasplib::gpio::GPIOPin>(this, line));
    }
    catch(rasplib::exception::Exception e)
    {
        if(_handler)
        {
            close(_handler);
            _handler = 0;
        };

        throw e;
    };
};



void GPIOChip::Close()
{
    if(_handler)
    {
        close(_handler);
        _handler = 0;
    };
};


bool GPIOChip::IsOpen()
{
    if(_handler)
        return true;

    return false;
};



int GPIOChip::GetHandler()
{
    return _handler;
};



GPIOPin& GPIOChip::Pin(unsigned short num)
{
    if(num >= _lines)
        throw rasplib::exception::Exception(101, "Requested line number is higher than available lines");

    return *_pins[num];
};


}
}
