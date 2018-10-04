


#ifndef __RASPLIB_GPIO_INTERRUPS_HPP__
#define __RASPLIB_GPIO_INTERRUPS_HPP__


#include <thread>
#include <vector>
#include <mutex>
#include <memory>
#include <gpio/pin.hpp>



namespace rasplib {
namespace gpio {




class Interrupts
{
private:
    std::unique_ptr<std::thread> _interrupts_thread;        //< Interrupts thread pointer
    std::mutex _thread_lock;                                //< Mutex to handle reading/writing data between threads

    bool _stop;                                             //< Stop interrupt thread when true

private:
    std::vector<GPIOPin*> _pins;                            //< Array of pointers to pins handled by interrupts
    std::vector<gpioevent_request*> _requests;              //< Array of event requests associated with pins

    /**
     * Callback function for interrupts
     *
     * @param self Pointer to Interrupts object
     */
    static void interrupts_handler(Interrupts *self);

    /**
     * Main function to handle interrupts
     */
    void handle();

    /**
     * Get request handler for a pin
     *
     * @param num_pin Pin number
     * @return Request handler
     */
    int request(int num_pin);

private:
    /**
     * Class constructor
     */
    Interrupts();

public:
    /**
     * Class destructor
     */
    ~Interrupts();

public:
    /**
     * Get Interrupts object.
     *
     * Only one object should exist in system
     */
    static Interrupts* get();

public:
    /**
     * Start generating interrupts for a pin
     *
     * @param pin Pointer to pin object
     */
    void start_interrupt(GPIOPin *pin);

    /**
     * Stop generating interrupts for a pin
     *
     * @param pin Pointer to pin object
     */
    void stop_interrupt(GPIOPin *pin);

    /**
     * Stop all interrupts and stop subsystem
     */
    void exit();
};



}
}



#endif
