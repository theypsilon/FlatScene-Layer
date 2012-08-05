#include "Event.h"
#include "EventBridge.h"

namespace FlatScene {

    Event::Event() : ResourceHandler(new EventBridge) {}

    Event::Event(EventBridge* ev) : ResourceHandler(ev) {}

    EventType::EventType Event::getType() const {
        return static_cast<EventType::EventType>(getRes().ev.type);
    }

    Key::Key Event::getSymbol() const {
        return static_cast<Key::Key>(getRes().ev.key.keysym.sym);
    }

}