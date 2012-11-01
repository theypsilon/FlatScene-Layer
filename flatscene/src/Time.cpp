#include "Time.h"
#include <chrono>
#include <thread>

#include <chrono>

namespace FlatScene {

Time::Time() {}

Time::~Time() {}

unsigned int Time::getTick() const {
    return _ticks;

}

unsigned int Time::setInterval(unsigned int msInterval) {
    auto aux = _msInterval;
    _msInterval = msInterval;
    return aux;
}

unsigned int Time::setFPS(unsigned int fpsInterval) {
    auto aux = setInterval(1000 / fpsInterval);
    return (1000 / aux);
}

int  Time::nextFrame() {
    typedef std::chrono::system_clock clock;

    auto ms = clock::now() + std::chrono::milliseconds(_msLast + _msInterval);

    while (ms > clock::now()) {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
    }
    _msLast = clock::now().time_since_epoch().count();
    _ticks++;

    return EXIT_SUCCESS;
}

void Time::reset(unsigned int tick) {
    _ticks = tick;
}

} // flatscene
