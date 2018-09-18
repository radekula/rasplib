#include <memory>
#include <thread>
#include <chrono>
#include <iostream>
#include <rasplib.hpp>




int main(int argc, char *argv[])
{
    try
    {
        rasplib::gpio::GPIODevice gpio;

        gpio.open("/dev/gpiochip0");

        gpio.pin(0).map_to_line(10);
        gpio.pin(1).map_to_line(9);
        gpio.pin(2).map_to_line(11);
        gpio.pin(3).map_to_line(5);
        gpio.pin(4).map_to_line(6);
        gpio.pin(5).map_to_line(13);
        gpio.pin(6).map_to_line(19);
        gpio.pin(7).map_to_line(26);

        int direction = -1;
        int current = 0;

        for(;;)
        {
            gpio.pin(current).set_state(true);
            gpio.pin(current - direction).set_state(false);

            if(current == 7)
                direction = -1;

            if(current == 0)
                direction = 1;

            current += direction;

            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }

        gpio.close();
    }
    catch (rasplib::Exception e)
    {
        std::cout << "Code: " << e.code() << " - " << e.what() << std::endl;
    };
};
