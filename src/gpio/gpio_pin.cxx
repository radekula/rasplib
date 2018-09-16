#include <sys/ioctl.h>
#include <linux/gpio.h>
#include <unistd.h>
#include <memory.h>
#include <string>
#include <exception/exception.hpp>
#include <gpio/gpio_chip.hpp>


#include <iostream>

namespace rasplib {
namespace gpio {




GPIOPin::GPIOPin()
{
    _line = -1;
    _chip = 0;
};




GPIOPin::GPIOPin(GPIOChip *chip, unsigned short line)
{
    _line = -1;
    _chip = 0;

    SetChip(chip);
    MapToLine(line);
};




GPIOPin::~GPIOPin()
{
    Release();
};




void GPIOPin::SetChip(GPIOChip *chip)
{
    _chip = chip;
};



void GPIOPin::MapToLine(unsigned short line)
{
    if(_line >= 0)
        Release();

    _line = (short) line;
};




void GPIOPin::SetState(bool state)
{
    if(!_chip)
        throw rasplib::exception::Exception(51, "Chip reference is not available");

    Release();

    struct gpiohandle_data data;
    memset(&data, 0, sizeof(data));

    _req.lineoffsets[0] = _line;
    _req.lines = 1;
    _req.flags = GPIOHANDLE_REQUEST_OUTPUT;

    int status = 0;

    status = ioctl(_chip->GetHandler(), GPIO_GET_LINEHANDLE_IOCTL, &_req);
    if(status < 0)
        throw rasplib::exception::Exception(52, "Unable to get gpio request");

    data.values[0] = state;

    status = ioctl(_req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
    if(status < 0)
        throw rasplib::exception::Exception(53, "Unable to write new line state to gpio device");
};




bool GPIOPin::GetState()
{
    return false;
};



void GPIOPin::Release()
{
    if(_req.fd)
        close(_req.fd);

    memset(&_req, 0, sizeof(_req));
};

}
}
