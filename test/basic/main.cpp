#include <memory>
#include <rasplib.hpp>


int main(int argc, char *argv[])
{
    auto gpio = std::make_shared<rasplib::gpio::GPIOChip>();
};
