#ifndef __EVENTHANDLER_H__
#define __EVENTHANDLER_H__

#include "FSTime.h"
#include <map>
#include <functional>
#include <memory>

class FSLibrary;

class FSEngine : public std::enable_shared_from_this<FSEngine> {
    friend class FSLibrary;
    bool initialized;
protected:
    int priority;

    virtual void onEvent(const SDL_Event& event);
    virtual void onInit();
    virtual void onExit();
    virtual void loop();
    virtual void onIdle();
    virtual void deselect();

public:
    bool done;

    FSEngine();
    virtual ~FSEngine();

    bool isInitialized();

    virtual void drawFrame();
};

#endif //#ifndef __EVENTHANDLER_H__
