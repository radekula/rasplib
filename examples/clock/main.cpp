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
        rasplib::clock::Clock clock;

        gpio.open("/dev/gpiochip0");
        gpio.pin(0).map_to_line(18);

        clock.init(&gpio.pin(0), 10);
        clock.start();

        std::this_thread::sleep_for(std::chrono::seconds(5));

        clock.stop();
    }
    catch (rasplib::Exception e)
    {
        std::cout << "Code: " << e.code() << " - " << e.what() << std::endl;
    };
};
