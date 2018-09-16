#ifndev __RASPLIB_GPIO_CHIP_HPP__
#define __RASPLIB_GPIO_CHIP_HPP__


#include <string>
#include <vector>
#include <gpio_line.hpp>



namespace rasplib {
namespace gpio {



class GPIOChip 
{
private:
    std::string _name;
    int _handler;
    std::vector<GPIOLine> _lines;

public:
    GPIOChip();
    ~GPIOChip();

public:
    void Open(std::string device);
    void Close();
    bool IsOpen();
};




#endif
