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

    // start interrupt thread on first use of Interrupts object
    _interrupts_thread = std::make_unique<std::thread>(interrupts_handler, this);
    _interrupts_thread->detach();
};



Interrupts::~Interrupts()
{
    exit();
};



Interrupts* Interrupts::get()
{
    static Interrupts _self;
    return &_self;
};



void Interrupts::start_interrupt(GPIOPin *pin)
{
    // lock access to pins and requests arrays
    std::lock_guard<std::mutex> guard(_thread_lock);

    _pins.push_back(pin);
    _requests.push_back(new gpioevent_request);
};



void Interrupts::stop_interrupt(GPIOPin *pin)
{
    // lock access to pins and requests arrays
    std::lock_guard<std::mutex> guard(_thread_lock);

    auto pos = _pins.begin();
    auto req = _requests.begin();

    // search for pin
    while(pos != _pins.end())
    {
        // if pin is found remove it with request
        if(*pos == pin)
        {
            _pins.erase(pos);

            if((*req)->fd)
                close((*req)->fd);

            _requests.erase(req);

            break;
        };

        pos++;
        req++;
    }

    if(pos == _pins.end())
        throw rasplib::Exception(INTERRUPT_NOT_FOUND, "Interrupt for pin not found");
};



void Interrupts::exit()
{
    // if thread is running stop it
    if(!_interrupts_thread)
        return;

    _stop = true;
    _interrupts_thread->join();

    // close all requests
    for(auto req : _requests)
        if(req->fd)
            close(req->fd);

    // clean all arrays
    _requests.clear();
    _pins.clear();
};




void Interrupts::interrupts_handler(Interrupts *self)
{
    self->handle();
}



int Interrupts::request(int num_pin)
{
    // lock access to pins and requests arrays
    std::lock_guard<std::mutex> guard(_thread_lock);

    // if request don't exist create new one
    if(!_requests[num_pin]->fd)
    {
        std::memset(_requests[num_pin], 0, sizeof(gpioevent_request));

        // only lines set for reading (OPEN DRAIN) can have interrupts
        _requests[num_pin]->lineoffset = _pins[num_pin]->line();
        _requests[num_pin]->handleflags |= GPIOHANDLE_REQUEST_OPEN_DRAIN;

        // we will be listening for changes in both edges
        _requests[num_pin]->eventflags |= GPIOEVENT_REQUEST_BOTH_EDGES;

        // call system function to set line to generate events
        auto rv = ioctl(_pins[num_pin]->device()->handler(), GPIO_GET_LINEEVENT_IOCTL, _requests[num_pin]);
    }

    // return request handler
    return _requests[num_pin]->fd;
};



//TODO: more atomic thread safe guard
//TODO: second mutex only for manipulating _pins array as pins cannot be removed or rearranged during loop iteration (other operations like searching are permited)
void Interrupts::handle()
{
    static gpioevent_data evdata;
    static timespec ts = {0, 1000};                                         //< timeout for waiting for events in each iteration

    // wait for events in loop until stop from parent thread is not set
    while(!_stop)
    {
        int num_pins = 0;
        pollfd *fds = 0;

        {
            std::lock_guard<std::mutex> guard(_thread_lock);

            // prepare lines handlers poll
            num_pins = _pins.size();
            fds = new pollfd[num_pins];
        }

        // get requests handlers for each line
        int pin_num = 0;
        for(auto iter : _pins)
        {
            fds[pin_num].fd = request(pin_num);
            fds[pin_num].events = POLLIN | POLLPRI;

            pin_num++;
        };

        // wait for events for all lines
        auto ret = ppoll(fds, num_pins, &ts, NULL);

        // handle line events
        for(pin_num = 0; pin_num < num_pins; pin_num++)
        {
            // detect if event is set for current line
            if(fds[pin_num].revents)
            {
                if(fds[pin_num].revents & POLLNVAL)
                    continue;

                // read data for event
                std::memset(&evdata, 0, sizeof(evdata));
                auto rd = read(fds[pin_num].fd, &evdata, sizeof(evdata));

                // if pin still exists call interrupt handler
                // as for now we cannot detect situation when pin is removed and
                std::lock_guard<std::mutex> guard(_thread_lock);
                if(pin_num < _pins.size())
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

