#ifndef __FS_LIBRARY_IMPL__
#define __FS_LIBRARY_IMPL__

#include "FSLibrary.h"

struct FSLibrary::LibraryImpl {
    std::shared_ptr<FSEngine> actualEngine;

    // Lista de motores a�adidas por el usuario
    std::list<std::shared_ptr<FSEngine>> engineIn;

#ifdef IN_FILE_ERROR
    bool errorsInSession;
#endif

    std::list<std::string> errors;

#ifdef DEBUGTEST
    bool debugging;
    int debugticks;
#endif

    void setActualEngine(std::shared_ptr<FSEngine> newEngineActive);

    int onMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2);
    void pendingMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2);

    std::string toStringErrorGL(GLenum e);

    static bool orderEngine(std::shared_ptr<FSEngine>&,std::shared_ptr<FSEngine>&);

    static void onExit();
};

#endif
