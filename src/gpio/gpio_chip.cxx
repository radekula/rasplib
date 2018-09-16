#include <gpio_chip.hpp>


std::string _name;
int _handler;
std::vector<GPIOLine> _lines;


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


void GPIOChip::IsOpen()
{
    if(_handler != 0)
        return true;

    return false;
};
