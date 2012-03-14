#ifndef __TIME_H__
#define __TIME_H__

#include <map>
#include <memory>
#include "SDL.h"
#include "FSdefinitions.h"

namespace {

    class FSEngine;

    class FSTime {
    private:

        unsigned int _msInterval;
        unsigned int _msLast;
        unsigned int _ticks;

    #ifdef MENSAJES_FPS
        unsigned int _fps;
        unsigned int _auxTimer;
    #endif

    public:
        FSTime();
        ~FSTime();
        unsigned int setInterval(unsigned int msInterval );
        unsigned int setFPS(unsigned int fpsInterval );
        int nextFrame();
        unsigned int getTick() const;
        void reset(unsigned int tick = 0);
    };

    extern FSTime Chrono;

} // flatscene

#endif
