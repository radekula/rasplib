#include <memory>
#include <thread>
#include <chrono>
#include <iostream>
#include <rasplib.hpp>




rasplib::gpio::GPIODevice gpio;



void handle_pin_events(rasplib::gpio::GPIOPin *pin, int edge)
{
    if(edge == rasplib::gpio::Edge::RISING)
    {
        std::cout << "Detected RISING edge" << std::endl;
        pin->get_device()->pin(1).set_state(true);
    }
    else if(edge == rasplib::gpio::Edge::FALLING)
    {
        std::cout << "Detected RISING edge" << std::endl;
        pin->get_device()->pin(1).set_state(false);
    }
    else
    {
        std::cout << "State not recognised" << std::endl;
    };
};



int main(int argc, char *argv[])
{
    try
    {
        rasplib::gpio::GPIODevice gpio;

        gpio.open("/dev/gpiochip0");

        gpio.pin(0).map_to_line(18);
        gpio.pin(1).map_to_line(17);

        gpio.pin(0).set_function(handle_pin_events);

        for(;;)
            std::this_thread::sleep_for(std::chrono::seconds(1));

        gpio.close();
    }
    catch (rasplib::Exception e)
    {
        std::cout << "Code: " << e.code() << " - " << e.what() << std::endl;
    };
};
