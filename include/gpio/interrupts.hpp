


#ifndef __RASPLIB_GPIO_INTERRUPS_HPP__
#define __RASPLIB_GPIO_INTERRUPS_HPP__


#include <thread>
#include <vector>
#include <gpio/pin.hpp>



namespace rasplib {
namespace gpio {



class Interrupts
{
private:
    std::thread *_interrupts_thread;

    bool _stop;

private:
    std::vector<GPIOPin *> _pins;
    static void interrupts_handler(Interrupts *self);

    void handle();

private:
    Interrupts();

public:
    ~Interrupts();

public:
    static Interrupts* get();

public:
    void start_interrupt(GPIOPin *pin);
    void stop_interrupt(GPIOPin *pin);
    void exit();
};



}
}



#endif
