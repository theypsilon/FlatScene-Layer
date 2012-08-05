#include "Event.h"
#include "EventBridge.h"

namespace FlatScene {

    Event::Event() : ResourceHandler(new EventBridge) {}

    EventType::EventType Event::getType() const {
        return static_cast<EventType::EventType>(getRes().ev.type);
    }

    Key::Key Event::getSymbol() const {
        return static_cast<Key::Key>(getRes().ev.key.keysym.sym);
    }

}