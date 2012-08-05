#ifndef FS_LIBRARY_IMPL__
#define FS_LIBRARY_IMPL__

#include "Library.h"

namespace FlatScene {

struct Library::LibraryImpl {
    Engine* actualEngine;

    // Lista de motores a�adidas por el usuario
    std::vector<std::unique_ptr<Engine>> engineIn;

#ifdef IN_FILE_ERROR
    bool errorsInSession;
#endif

    std::list<std::string> errors;

    std::list<std::function<void()>> endTasks;

#ifdef DEBUGTEST
    bool debugging;
    int debugticks;
#endif

    void setActualEngine(Engine* newEngineActive);

    static void onExit();

    void sort(std::vector<std::unique_ptr<Engine>>& v);
};

} // flatscene

#endif
