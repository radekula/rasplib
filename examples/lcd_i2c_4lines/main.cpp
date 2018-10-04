#include <memory>
#include <thread>
#include <chrono>
#include <iostream>
#include <rasplib.hpp>



int main(int argc, char *argv[])
{
    try
    {
        rasplib::display::Alphanumeric lcd(20, 4, 20);

        lcd.init(1, std::bitset<8>(0x27));
        lcd.set_mode(true, false, false);

        lcd.clean();

        int loop = 0;
        while(loop < 10)
        {
            lcd.print("Raspberry Pi 3B+\n"
                      " Test display 1\n"
                      "Line 3 test\n"
                      "Line 4 test");

            std::this_thread::sleep_for(std::chrono::seconds(5));

            for(int i = 0; i < 10; i++)
            {
                std::string test(std::string("      00:0") + std::to_string(i) + "     \n" +
                                             " Test display 2\n"
                                             "Line 3 test 2\n"
                                             "Line 4 test 2");
                lcd.print(test);
                std::this_thread::sleep_for(std::chrono::seconds(1));
            };

            lcd.print("Freq: 3200 [MHz]\n"
                      "Temp:   46 [C]\n"
                      "Memory: 34 [%]\n"
                      "Load:   0.21");

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
