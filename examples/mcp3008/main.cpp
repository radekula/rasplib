#include <memory>
#include <thread>
#include <chrono>
#include <iostream>
#include <rasplib.hpp>
#include <library/mcp300x.hpp>





int main(int argc, char *argv[])
{
    rasplib::gpio::GPIODevice gpio;
    rasplib::clock::Clock clock;
    rasplib::spi::SPIDevice spi;
    rasplib::library::MCP300x mcp;

    short port = 0;

    if(argc > 1)
        port = std::stoi(argv[1]);

    try
    {
        // Setup GPIO device
        gpio.open("/dev/gpiochip0");
        gpio.pin(0).map_to_line(19);   // CS
        gpio.pin(1).map_to_line(13);   // D-in
        gpio.pin(2).map_to_line(6);    // CLK
        gpio.pin(3).map_to_line(12);   // D-out

        // Setup clock
        clock.init(&gpio.pin(2), 200000);
        clock.start();

        // Setup SPI device
        spi.init(&gpio.pin(0), &gpio.pin(1), &gpio.pin(3), &clock);

        // Setup MCP device
        mcp.set_type(rasplib::library::MCP300x_TYPE::MCP3008);
        mcp.set_spi(&spi);

        std::cout << "\x1B[2J\x1B[H";

        for(int i = 0; ; i++)
        {
            std::cout << "\r" << std::to_string(mcp.read(port, false, true));
            std::cout << " [Test " << std::to_string(i + 1) << "]";
            std::cout.flush();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        };
    }
    catch (rasplib::Exception e)
    {
        std::cout << "Code: " << e.code() << " - " << e.what() << std::endl;
    };

    clock.stop();
};
