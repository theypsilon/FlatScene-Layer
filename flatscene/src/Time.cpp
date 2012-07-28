#include "Time.h"
#include "Screen.h"
#include "Library.h"

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
    int ret = EXITO;

    while ((_msLast + _msInterval) > SDL_GetTicks()) {
        SDL_Delay(1);
    }
    _msLast = SDL_GetTicks();
    _ticks++;

#ifdef MENSAJES_FPS
    _fps++;
    if (SDL_GetTicks() > _auxTimer + 1000) {
        _auxTimer=SDL_GetTicks();
        //Writer::I().erase(Writer::I().line(0,5,5,"FPS: %d ",_fps),true);
        _fps=0;
    }
    
#endif

    return ret;
}

void Time::reset(unsigned int tick) {
    _ticks = tick;
}

// TODO : Liberar los espacios de memoria correspondiente a los engines cuando estos sean eliminados. 
//              No es prioritario porque la creaci�n y destrucci�n de engines durante la ejecuci�n deber�a ser m�nima, y la memoria reservada es muy peque�a.

Time Chrono;

} // flatscene
