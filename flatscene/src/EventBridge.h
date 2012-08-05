#ifndef FS_EVENT_BRIDGE
#define FS_EVENT_BRIDGE

#include "Includes.h"

#include <vector>

namespace FlatScene {

    class EventBridge {
        SDL_Event ev;

        friend class Event;
        friend class Engine;
        friend std::vector<Event> PollEvents();
    };

} // FlatScene

#endif