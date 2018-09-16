#include <memory>
#include <thread>
#include <chrono>
#include <iostream>
#include <rasplib.hpp>



void init_gpio(rasplib::gpio::GPIOChip &gpio)
{
    // data pins
    gpio.Pin(0).MapToLine(26);
    gpio.Pin(1).MapToLine(19);
    gpio.Pin(2).MapToLine(13);
    gpio.Pin(3).MapToLine(6);
    gpio.Pin(4).MapToLine(5);
    gpio.Pin(5).MapToLine(11);
    gpio.Pin(6).MapToLine(9);
    gpio.Pin(7).MapToLine(10);

    // control pins
    gpio.Pin(8).MapToLine(21);
    gpio.Pin(9).MapToLine(20);
};



void init_lcd(rasplib::display::Alphanumeric &lcd, rasplib::gpio::GPIOChip &gpio)
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

    lcd.Init(&gpio, command_pin, send_pin, data_pins);
    lcd.SetMode(2, false, false);

    lcd.Clean();
};


int main(int argc, char *argv[])
{
    try
    {
        rasplib::gpio::GPIOChip gpio;

        gpio.Open("/dev/gpiochip0");
        init_gpio(gpio);

        rasplib::display::Alphanumeric lcd(16, 2);

        init_lcd(lcd, gpio);

        int loop = 0;
        while(loop < 10)
        {
            lcd.Print("Raspberry Pi 3B+\n"
                      " Test display 1 ");

            std::this_thread::sleep_for(std::chrono::seconds(5));

            for(int i = 0; i < 10; i++)
            {
                std::string test(std::string("      00:0") + std::to_string(i) + "     \n" +
                                             " Test display 2 ");
                lcd.Print(test);
                std::this_thread::sleep_for(std::chrono::seconds(1));
            };

            lcd.Print("Freq: 3200 [MHz]\n"
                      "Temp:   46 [C]");

            std::this_thread::sleep_for(std::chrono::seconds(5));
            loop++;
        };

        lcd.Clean();

        gpio.Close();
    }
    catch (rasplib::exception::Exception e)
    {
        std::cout << "Code: " << e.code() << " - " << e.what() << std::endl;
    };
};
