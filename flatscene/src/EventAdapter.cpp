#include "EventAdapter.h"
#include "EventBridge.h"
#include "Includes.h"

namespace FlatScene {

    void FreeAllEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)==1);
    }

    std::vector<Event> PollEvents() {
        std::vector<Event> events;

        for (Event ev; SDL_PollEvent(&ev.getRes().ev) ; ev = Event()) {
            events.push_back(ev);
        }
        return events;
    }

} // FlatScene