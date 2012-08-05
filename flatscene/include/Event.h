#ifndef FS_EVENT__
#define FS_EVENT__

#include "ResourceHandler.h"
#include "EventInfo.h"

namespace FlatScene {

    class EventBridge;

    class Event : public ResourceHandler<EventBridge,RefCountMemoryPolicy<EventBridge> > {
    public:
        EventType::EventType getType() const;
        Key::Key getSymbol() const;
    private:
        Event(EventBridge* ev);
        Event();
        friend class Engine;
        friend std::vector<Event> PollEvents();
    };

} // FlatScene

#endif // FS_EVENT__