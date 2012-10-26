#ifndef FS_LIBRARY_IMPL__
#define FS_LIBRARY_IMPL__

#include "Library.h"

namespace FlatScene {

struct Library::LibraryImpl {

#ifdef IN_FILE_ERROR
    bool errorsInSession;
#endif

    std::list<std::string> errors;

    std::list<std::function<void()>> endTasks;

#ifdef DEBUGTEST
    bool debugging;
    int debugticks;
#endif

    static void onExit();
};

} // flatscene

#endif
