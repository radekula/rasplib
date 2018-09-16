#include <gpio/gpio_chip.hpp>


namespace rasplib {
namespace gpio {




GPIOChip::GPIOChip()
{
    _name = "unknown";
    _handler = 0;
};



GPIOChip::~GPIOChip()
{
    if(IsOpen())
        Close();
};




void GPIOChip::Open(std::string device)
{
};



void GPIOChip::Close()
{
};


bool GPIOChip::IsOpen()
{
    if(_handler != 0)
        return true;

    return false;
};




}
}
