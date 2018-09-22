


#ifndef __RASPLIB_GPIO_INTERRUPS_HPP__
#define __RASPLIB_GPIO_INTERRUPS_HPP__


#include <thread>
#include <vector>
#include <mutex>
#include <gpio/pin.hpp>



namespace rasplib {
namespace gpio {



class Interrupts
{
private:
    std::thread *_interrupts_thread;

    std::mutex _thread_lock;

    bool _stop;

private:
    std::vector<GPIOPin*> _pins;
    std::vector<gpioevent_request*> _requests;

    static void interrupts_handler(Interrupts *self);

    void handle();
    int get_request(int num_pin);

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
