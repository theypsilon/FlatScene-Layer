#ifndef __FS_LIBRARY_IMPL__
#define __FS_LIBRARY_IMPL__

#include "FSLibrary.h"

struct FSLibrary::LibraryImpl {
    FSEngine* actualEngine;

    // Lista de motores a�adidas por el usuario
    list<FSEngine*> engineIn;
    // Lista de motores ejecutados por orden del usuario sin ser a�adidos (mediante mensajeria).
    list<FSEngine*> engineOut;

#ifdef IN_FILE_ERROR
    bool errorsInSession;
#endif

    list<string> errors;

#ifdef DEBUGTEST
    bool debugging;
    int debugticks;
#endif


    void setLibrary(FSLibrary* pTheLib);
    void setActualEngine(FSEngine* newEngineActive);
    void onExit();

    int onMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2);
    void pendingMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2);

    string toStringErrorGL(GLenum e);

    static bool orderEngine(FSEngine*,FSEngine*);
};

#endif
