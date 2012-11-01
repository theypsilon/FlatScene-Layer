#include "Time.h"
#include "Screen.h"
#include "Library.h"

#include "Includes.h"
#include <chrono>

#include <chrono>

namespace FlatScene {

Time::Time() {
#ifdef MENSAJES_FPS
        _fps=0;
        _auxTimer=0;
#endif
}

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
    using std::chrono::duration_cast;
    using std::chrono::nanoseconds;
    typedef std::chrono::system_clock clock;
    typedef std::chrono::time_point<std::chrono::system_clock> time;


    int ret = EXIT_SUCCESS;

    auto ms = clock::now() + std::chrono::milliseconds(_msLast + _msInterval);

    while (ms > clock::now()) {
        SDL_Delay(1);
    }
    _msLast = clock::now().time_since_epoch().count();
    _ticks++;

#ifdef MENSAJES_FPS
    _fps++;
    if (clock::now() > _auxTimer + 1000) {
        _auxTimer=clock::now();
        //Writer::I().erase(Writer::I().line(0,5,5,"FPS: %d ",_fps),true);
        _fps=0;
    }
    
#endif

    return ret;
}

void Time::reset(unsigned int tick) {
    _ticks = tick;
}

} // flatscene
