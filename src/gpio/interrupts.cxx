#include <algorithm>
#include <poll.h>
#include <unistd.h>
#include <cstring>
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <gpio/interrupts.hpp>
#include <gpio/pin.hpp>
#include <gpio/device.hpp>
#include <exception/exception.hpp>




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

    for(auto iter : _requests)
    {
        if(iter->fd)
            close(iter->fd);

        delete iter;
    };
};



Interrupts* Interrupts::get()
{
    static Interrupts _self;

    return &_self;
};



void Interrupts::start_interrupt(GPIOPin *pin)
{
    std::lock_guard<std::mutex> guard(_thread_lock);

    _pins.push_back(pin);
    _requests.push_back(new gpioevent_request);
};



void Interrupts::stop_interrupt(GPIOPin *pin)
{
    std::lock_guard<std::mutex> guard(_thread_lock);

    auto pos = _pins.begin();
    auto req = _requests.begin();

    while(pos != _pins.end())
    {
        if(*pos == pin)
        {
            _pins.erase(pos);

            if((*req)->fd)
                close((*req)->fd);

            _requests.erase(req);
        };
    }

    if(pos == _pins.end())
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



int Interrupts::get_request(int num_pin)
{
    std::lock_guard<std::mutex> guard(_thread_lock);

    if(!_requests[num_pin]->fd)
    {
        std::memset(_requests[num_pin], 0, sizeof(gpioevent_request));

        _requests[num_pin]->lineoffset = _pins[num_pin]->get_line();
        _requests[num_pin]->handleflags |= GPIOHANDLE_REQUEST_OPEN_DRAIN;

        _requests[num_pin]->eventflags |= GPIOEVENT_REQUEST_BOTH_EDGES;

        auto rv = ioctl(_pins[num_pin]->get_device()->get_handler(), GPIO_GET_LINEEVENT_IOCTL, _requests[num_pin]);
    }

    return _requests[num_pin]->fd;
};



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
            fds[pin_num].fd = get_request(pin_num);
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

                std::lock_guard<std::mutex> guard(_thread_lock);
                _pins[pin_num]->handle_interrupt(evdata.id == GPIOEVENT_EVENT_RISING_EDGE
                                                ? Edge::RISING : Edge::FALLING);

                if (!--ret)
                    break;
            }
        }
    };
};




}
}

