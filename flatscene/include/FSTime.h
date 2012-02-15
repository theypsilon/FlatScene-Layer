#ifndef __TIME_H__
#define __TIME_H__

#include <map>
#include <memory>
#include "SDL.h"
#include "FSdefinitions.h"

class FSEngine;

class FSTime {
private:
    struct STimeData {
        unsigned int msInterval;
        unsigned int msLast;
        unsigned int frameCount;

        STimeData() : msInterval(16), msLast(0), frameCount(0) {
        };
    };

    mutable std::map<std::shared_ptr<FSEngine>,STimeData> fc;

#ifdef MENSAJES_FPS
    unsigned int fps;
    unsigned int auxTimer;
    std::map<std::shared_ptr<FSEngine>,int> adminText;
#endif

    mutable STimeData* actTime;
    mutable std::shared_ptr<FSEngine> admin;

    bool all;
    unsigned int allMsInterval;
public:
    FSTime();
    ~FSTime();
    unsigned int setInterval(unsigned int msNew, bool all=false);
    unsigned int setFPS(unsigned int fpsNew, bool all=false);
    int nextFrame();
    unsigned int getTick() const;

    bool isTimeForAll() const;
};

extern FSTime Chrono;

#endif
