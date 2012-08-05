#ifndef FS_EVENT__
#define FS_EVENT__

#include "ResourceHandler.h"
#include "EventInfo.h"

#include <vector>

namespace FlatScene {

    class EventBridge;

    class Event : public ResourceHandler<EventBridge,RefCountMemoryPolicy<EventBridge> > {
    public:
        EventType::EventType getType() const;
        Key::Key getSymbol() const;
    private:
        Event();
        friend class Engine;
        friend std::vector<Event> PollEvents();
    };

} // FlatScene

#endif // FS_EVENT__