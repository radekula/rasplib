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

        gpio.pin(0).map_to_line(18);
        gpio.pin(1).map_to_line(17);

        for(;;)
        {
            auto value = gpio.pin(0).state();
            gpio.pin(1).set_state(value);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        gpio.close();
    }
    catch (rasplib::Exception e)
    {
        std::cout << "Code: " << e.code() << " - " << e.what() << std::endl;
    };
};
