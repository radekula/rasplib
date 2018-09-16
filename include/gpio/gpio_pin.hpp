#ifndef __RASPLIB_GPIO_PIN_HPP__
#define __RASPLIB_GPIO_PIN_HPP__


#include <memory>
#include <linux/gpio.h>


namespace rasplib {
namespace gpio {


class GPIOChip;


class GPIOPin
{
private:
    GPIOChip *_chip;
    short _line;
    gpiohandle_request _req;

private:
    void Release();

public:
    GPIOPin();
    GPIOPin(GPIOChip *chip, unsigned short line);
    ~GPIOPin();

public:
    void SetChip(GPIOChip *chip);
    void MapToLine(unsigned short line);

public:
    void SetState(bool state);
    bool GetState();
};




}
}

#endif
