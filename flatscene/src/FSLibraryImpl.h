#ifndef __FS_LIBRARY_IMPL__
#define __FS_LIBRARY_IMPL__

#include "FSLibrary.h"

struct FSLibrary::LibraryImpl {
    FSEngine* actualEngine;

    // Lista de motores a�adidas por el usuario
    std::vector<std::unique_ptr<FSEngine>> engineIn;

#ifdef IN_FILE_ERROR
    bool errorsInSession;
#endif

    std::list<std::string> errors;

    std::list<std::function<void()>> endTasks;

#ifdef DEBUGTEST
    bool debugging;
    int debugticks;
#endif

    void setActualEngine(FSEngine* newEngineActive);

    std::string toStringErrorGL(GLenum e);

    static bool orderEngine(std::unique_ptr<FSEngine>&,std::unique_ptr<FSEngine>&);

    static void onExit();
};

#endif
