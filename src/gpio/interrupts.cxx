#include <algorithm>
#include <poll.h>
#include <unistd.h>
#include <cstring>
#include <sys/ioctl.h>
#include <gpio/interrupts.hpp>
#include <gpio/pin.hpp>
#include <gpio/device.hpp>
#include <exception/exception.hpp>


#include <iostream>


namespace rasplib {
namespace gpio {



Interrupts::Interrupts()
{
    _stop = false;

    _interrupts_thread = new std::thread(interrupts_handler, this);
    _interrupts_thread->detach();
};



Interrupts::~Interrupts()
{
    _stop = true;
    _interrupts_thread->join();

    if(_interrupts_thread)
        delete _interrupts_thread;
};



Interrupts* Interrupts::get()
{
    static Interrupts _self;

    return &_self;
};



void Interrupts::start_interrupt(GPIOPin *pin)
{
    _pins.push_back(pin);
};



void Interrupts::stop_interrupt(GPIOPin *pin)
{
    auto pos = std::find(_pins.begin(), _pins.end(), pin);
    if (pos != _pins.end())
        _pins.erase(pos);
    else
        throw rasplib::Exception(INTERRUPT_NOT_FOUND, "Interrupt for pin not found");
};



void Interrupts::exit()
{
    if(!_interrupts_thread)
        return;

    _stop = true;
    _interrupts_thread->join();
};





void Interrupts::interrupts_handler(Interrupts *self)
{
    self->handle();
}




//TODO: thread safe
void Interrupts::handle()
{
    static gpioevent_data evdata;
    static timespec ts = {0, 1000};

    while(!_stop)
    {
        auto num_pins = _pins.size();

        struct pollfd fds[num_pins];

        int pin_num = 0;
        for(auto iter : _pins)
        {
            struct gpioevent_request req;
            std::memset(&req, 0, sizeof(req));

            req.lineoffset = iter->get_line();
            req.handleflags |= GPIOHANDLE_REQUEST_OPEN_DRAIN;

            req.eventflags |= GPIOEVENT_REQUEST_RISING_EDGE;
            req.eventflags |= GPIOEVENT_REQUEST_FALLING_EDGE;

            auto rv = ioctl(iter->get_device()->get_handler(), GPIO_GET_LINEEVENT_IOCTL, &req);

            sigset_t mask;
            int sigfd;

            sigemptyset(&mask);
            sigaddset(&mask, SIGTERM);
            sigaddset(&mask, SIGINT);

            rv = sigprocmask(SIG_BLOCK, &mask, NULL);
            if (rv < 0)
                throw rasplib::Exception(SIGNAL_MASK_CREATE_ERROR, "Error creating signal mask");

            sigfd = signalfd(-1, &mask, 0);

            fds[pin_num].fd = sigfd;
            fds[pin_num].events = POLLIN | POLLPRI;

            pin_num++;
        };

        auto ret = ppoll(fds, num_pins, &ts, NULL);

        for(pin_num = 0; pin_num < num_pins; pin_num++) 
        {
            if(fds[pin_num].revents) 
            {
                if(fds[pin_num].revents & POLLNVAL)
                    continue;

                std::memset(&evdata, 0, sizeof(evdata));

                auto rd = read(fds[pin_num].fd, &evdata, sizeof(evdata));
                _pins[pin_num]->handle_interrupt(evdata.id == GPIOEVENT_EVENT_RISING_EDGE
                                                ? Edge::RISING : Edge::FALLING);

                if(fds[pin_num].fd)
                    close(fds[pin_num].fd);

                if (!--ret)
                    break;
            }
        }
    };
};




}
}

