#include <memory>
#include <thread>
#include <chrono>
#include <iostream>
#include <rasplib.hpp>




int main(int argc, char *argv[])
{
    try
    {
        rasplib::gpio::GPIOChip gpio;

        gpio.Open("/dev/gpiochip0");

        gpio.Pin(0).MapToLine(10);
        gpio.Pin(1).MapToLine(9);
        gpio.Pin(2).MapToLine(11);
        gpio.Pin(3).MapToLine(5);
        gpio.Pin(4).MapToLine(6);
        gpio.Pin(5).MapToLine(13);
        gpio.Pin(6).MapToLine(19);
        gpio.Pin(7).MapToLine(26);

        int direction = -1;
        int current = 0;

        for(;;)
        {
            gpio.Pin(current).SetState(true);
            gpio.Pin(current - direction).SetState(false);

            if(current == 7)
                direction = -1;

            if(current == 0)
                direction = 1;

            current += direction;

            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }

        gpio.Close();
    }
    catch (rasplib::exception::Exception e)
    {
        std::cout << "Code: " << e.code() << " - " << e.what() << std::endl;
    };
};
