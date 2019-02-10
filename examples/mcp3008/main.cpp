#include <memory>
#include <thread>
#include <chrono>
#include <iostream>
#include <rasplib.hpp>



class MCP3008
{
private:
    rasplib::gpio::GPIOPin *_cs;
    rasplib::gpio::GPIOPin *_d_in;
    rasplib::gpio::GPIOPin *_d_out;
    rasplib::clock::Clock *_clock;
    unsigned short _value;

private:
    unsigned short _input;
    unsigned short _cycle;
    bool _read_done;

public:
    MCP3008()
    {
        _input = 0;
        _value = 0;
        _cycle = 0;
        _read_done = false;
    };

    ~MCP3008() {};

private:
    static void clock_handler(rasplib::clock::Clock *Clock, void *self)
    {
        ((MCP3008 *) self)->handle();
    }

    void handle()
    {
        if(_read_done)
            return;

        _cycle++;

        if(_cycle < 8)
        {
            // first 7 bits are 0
            _d_in->set_state(0);
            return;
        }
        else if(_cycle == 8)
        {
            // 8th bit is start bit
            _d_in->set_state(1);
            return;
        }
        else if(_cycle > 8 && _cycle < 13)
        {
            // 9th, 10th, 11th, 12th bits set reading parameters
            switch(_cycle)
            {
                case 9:
                    _d_in->set_state(1);
                    break;
                case 10:
                    _d_in->set_state(_input & 1);
                    break;
                case 11:
                    _d_in->set_state(_input & 2);
                    break;
                case 12:
                    _d_in->set_state(_input & 4);
                    break;
            };
            return;
        }
        else if(_cycle > 12 && _cycle < 14)
        {
            // do nothing - waiting for results
            return;
        }
        else if(_cycle > 13 && _cycle < 24)
        {
            // reading result
std::cout << std::to_string(_d_out->state());
            _value |= _d_out->state() << (25 - _cycle);
            return;
        } 
        else if(_cycle > 23)
        {
            _cycle = 0;
            _read_done = true;
std::cout << "\n";
        };
    };

public:
    void init(rasplib::gpio::GPIOPin *cs,
              rasplib::gpio::GPIOPin *d_in,
              rasplib::gpio::GPIOPin *d_out,
              rasplib::clock::Clock *clock)
    {
        _cs = cs;
        _d_in = d_in;
        _d_out = d_out;
        _clock = clock;
    };

    short read(unsigned short input)
    {
        if(input > 7)
            return -1;

        _input = input;
        _cycle = 0;
        _value = 0;
        _read_done = false;

        _cs->set_state(1);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        _cs->set_state(0);

        _clock->set_function(clock_handler, false, (void *) this);

        while(true)
        {
            std::this_thread::sleep_for(std::chrono::nanoseconds(100));
            if(_read_done)
                break;
        };
        _cs->set_state(1);
        _clock->remove_function(false);

        return _value;
    };
};



int main(int argc, char *argv[])
{
    try
    {
        rasplib::gpio::GPIODevice gpio;
        rasplib::clock::Clock clock;

        gpio.open("/dev/gpiochip0");
        gpio.pin(0).map_to_line(23);   // CS
        gpio.pin(1).map_to_line(27);   // D-in
        gpio.pin(2).map_to_line(18);   // CLK
        gpio.pin(3).map_to_line(17);   // D-out

        clock.init(&gpio.pin(2), 25000);
        clock.start();

        MCP3008 mcp;
        mcp.init(&gpio.pin(0), &gpio.pin(1), &gpio.pin(3), &clock);

        for(int i = 0; i < 100; i++)
        {
            std::cout << "Test " << std::to_string(i + 1) << ":" << std::endl;
            std::cout << std::to_string(mcp.read(0)) << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        };

        clock.stop();
    }
    catch (rasplib::Exception e)
    {
        std::cout << "Code: " << e.code() << " - " << e.what() << std::endl;
    };
};
