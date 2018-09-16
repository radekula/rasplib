#ifndef __RASPLIB_GPIO_CHIP_HPP__
#define __RASPLIB_GPIO_CHIP_HPP__


#include <string>
#include <vector>
#include <memory>
#include <gpio/gpio_pin.hpp>



namespace rasplib {
namespace gpio {



class GPIOChip 
{
private:
    int _handler;

private:
    std::string _name;
    std::string _label;
    unsigned short _lines;

private:
    std::vector<std::shared_ptr<GPIOPin>> _pins;

public:
    GPIOChip();
    ~GPIOChip();

public:
    std::string Name();
    std::string Label();
    unsigned short Lines();

public:
    void Open(std::string device);
    void Close();
    bool IsOpen();

    int GetHandler();

public:
    GPIOPin& Pin(unsigned short num);
};



}
}

#endif
