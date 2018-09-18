#include <memory>
#include <thread>
#include <chrono>
#include <iostream>
#include <rasplib.hpp>



void init_gpio(rasplib::gpio::GPIODevice &gpio)
{
    // data pins
    gpio.pin(0).map_to_line(26);
    gpio.pin(1).map_to_line(19);
    gpio.pin(2).map_to_line(13);
    gpio.pin(3).map_to_line(6);
    gpio.pin(4).map_to_line(5);
    gpio.pin(5).map_to_line(11);
    gpio.pin(6).map_to_line(9);
    gpio.pin(7).map_to_line(10);

    // control pins
    gpio.pin(8).map_to_line(21);
    gpio.pin(9).map_to_line(20);
};



void init_lcd(rasplib::display::Alphanumeric &lcd, rasplib::gpio::GPIODevice &gpio)
{
    std::vector<unsigned short> data_pins;
    data_pins.push_back(0);
    data_pins.push_back(1);
    data_pins.push_back(2);
    data_pins.push_back(3);
    data_pins.push_back(4);
    data_pins.push_back(5);
    data_pins.push_back(6);
    data_pins.push_back(7);

    unsigned short command_pin = 8;
    unsigned short send_pin = 9;

    lcd.init(&gpio, command_pin, send_pin, data_pins);
    lcd.set_mode(2, false, false);

    lcd.clean();
};


int main(int argc, char *argv[])
{
    try
    {
        rasplib::gpio::GPIODevice gpio;

        gpio.open("/dev/gpiochip0");
        init_gpio(gpio);

        rasplib::display::Alphanumeric lcd(16, 2);

        init_lcd(lcd, gpio);

        int loop = 0;
        while(loop < 10)
        {
            lcd.print("Raspberry Pi 3B+\n"
                      " Test display 1 ");

            std::this_thread::sleep_for(std::chrono::seconds(5));

            for(int i = 0; i < 10; i++)
            {
                std::string test(std::string("      00:0") + std::to_string(i) + "     \n" +
                                             " Test display 2 ");
                lcd.print(test);
                std::this_thread::sleep_for(std::chrono::seconds(1));
            };

            lcd.print("Freq: 3200 [MHz]\n"
                      "Temp:   46 [C]");

            std::this_thread::sleep_for(std::chrono::seconds(5));
            loop++;
        };

        lcd.clean();

        gpio.close();
    }
    catch (rasplib::Exception e)
    {
        std::cout << "Code: " << e.code() << " - " << e.what() << std::endl;
    };
};
