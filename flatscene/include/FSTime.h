#ifndef __TIME_H__
#define __TIME_H__

#include <map>
#include <memory>
#include "FSdefinitions.h"

namespace flatscene {

    class Engine;

    class Time {
    private:

        unsigned int _msInterval;
        unsigned int _msLast;
        unsigned int _ticks;

    #ifdef MENSAJES_FPS
        unsigned int _fps;
        unsigned int _auxTimer;
    #endif

    public:
        Time();
        ~Time();
        unsigned int setInterval(unsigned int msInterval );
        unsigned int setFPS(unsigned int fpsInterval );
        int nextFrame();
        unsigned int getTick() const;
        void reset(unsigned int tick = 0);
    };

    extern Time Chrono;

} // flatscene

#endif
