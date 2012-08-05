#ifndef FS_EVENT_ADAPTER__
#define FS_EVENT_ADAPTER__

#include <vector>
#include "Event.h"

namespace FlatScene {

    void FreeAllEvents();
    std::vector<Event> PollEvents();

}

#endif // FS_EVENT_ADAPTER__