#include "Time.h"
#include "Screen.h"
#include "Library.h"

#include "Includes.h"

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
    typedef std::chrono::steady_clock clock;


    int ret = EXIT_SUCCESS;

    while ((_msLast + _msInterval) > clock::now().time_since_epoch().count()) {
        SDL_Delay(1);
    }
    _msLast = (unsigned int) clock::now().time_since_epoch().count();
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
