//include guards
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

//include message handler(base class)

#include "FSMessageHandler.h"

#include "FSScreen.h"
#include "FSEngine.h"

#include "FSImages.h"
#include "FSControlMultiverse.h"
#include "FSWriter.h"
#include "FSTime.h"

#include "FSCamera.h"
#include "FSUniverse.h"
#include "FSActor.h"

#include <stdlib.h>
#include "FSparserXML.h"

enum TypeError {
    TE_standard,
    TE_fileExists,
    TE_controlViolation,
    TE_SDL_NOMSG,
    TE_SDL_MSG,
    TE_OPENGL_NOMSG,
    TE_OPENGL_MSG
};


class FSLibrary : public FSMessageHandler  
{
private:

    static FSLibrary* s_pTheLibrary;

    FSEngine* actualEngine;

    // Lista de motores a�adidas por el usuario
    list<FSEngine*> engineIn;

    // Lista de motores ejecutados por orden del usuario sin ser a�adidos (mediante mensajeria).
    list<FSEngine*> engineOut;

#ifdef IN_FILE_ERROR
    bool errorsInSession;
#endif

    static list<string> errors;

#ifdef DEBUGTEST

    static bool debugging;
    static int debugticks;

#endif


    static void setLibrary(FSLibrary* pTheLib);

    static void setActualEngine(FSEngine* newEngineActive);

    friend class FSEngine;


    FSLibrary(bool xmlconfig =false);

    FSLibrary( int width , int height , int bpp , bool fullscreen, bool doublebuff=true ) ;


    static void onExit();

    int onMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2);
    void pendingMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2);

    static string toStringErrorGL(GLenum e);

    virtual ~FSLibrary();

public:

    static int startLibrary(bool xmlconfig);

    static int startLibrary( int width , int height , int bpp , bool fullscreen, bool doublebuff=true ) ;

    static int processEngines();


    inline static FSLibrary* getLibrary() {
        return s_pTheLibrary;
    }

    inline static FSEngine* getActualEngine() {
        return getLibrary()?getLibrary()->actualEngine:NULL;
    }

    static int addEngine(FSEngine* engine,int priority);

    static Uint32 MSGID_Exit;
    static Uint32 MSGID_Restart;
    static Uint32 MSGID_RunEngine;
    static Uint32 MSGID_ReloadEngine;
    static Uint32 MSGID_ChangeEngine;
    static Uint32 MSGID_KillEngine;

    static void Error (const char*,TypeError e=TE_standard);
    static void Error (std::string,TypeError e=TE_standard);
    static void Error (char*,TypeError e=TE_standard);

    static string readLastError();
    static string popError();

#ifdef DEBUGTEST

    static void debug(bool startdebug, const char* warning=NULL);
    static bool inDebug();

#endif

    bool static orderEngine(FSEngine*,FSEngine*);

};

#define SINERROR "|-| No error"

#endif //#ifndef __APPLICATION_H__
