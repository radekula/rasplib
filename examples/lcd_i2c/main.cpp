#include <memory>
#include <thread>
#include <chrono>
#include <iostream>
#include <rasplib.hpp>



void init_lcd(rasplib::display::Alphanumeric &lcd)
{
    lcd.init(1, std::bitset<8>(0x27));
    lcd.set_mode(2, false, false);

    lcd.clean();
};


int main(int argc, char *argv[])
{
    try
    {
        rasplib::display::Alphanumeric lcd(16, 2);

        init_lcd(lcd);

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
    }
    catch (rasplib::Exception e)
    {
        std::cout << "Code: " << e.code() << " - " << e.what() << std::endl;
    };
};
