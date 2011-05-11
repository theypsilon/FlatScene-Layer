//include guards
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

//include message handler(base class)

#include "FSMessageHandler.h"

#include "FSScreen.h"
#include "FSEngine.h"

#include "FSControlImages.h"
#include "FSControlMultiverse.h"
#include "FSControlOutputText.h"
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


class CLibrary : public CMessageHandler  
{
private:
	
	static CLibrary* s_pTheLibrary;

	CEngine* actualEngine;

	// Lista de motores añadidas por el usuario
	list<CEngine*> engineIn;

	// Lista de motores ejecutados por orden del usuario sin ser añadidos (mediante mensajeria).
	list<CEngine*> engineOut;

#ifdef IN_FILE_ERROR
	bool errorsInSession;
#endif

	static list<string> errors;

#ifdef DEBUGTEST

	static bool debugging;
	static int debugticks;

#endif

	
	static void setLibrary(CLibrary* pTheLib);

	static void setActualEngine(CEngine* newEngineActive);

	friend class CEngine;

	
	CLibrary(bool xmlconfig =false);

	CLibrary( int width , int height , int bpp , bool fullscreen, bool doublebuff=true ) ;

	
	static void onExit();

	int onMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2);
	void pendingMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2);

	static string toStringErrorGL(GLenum e);
	
	virtual ~CLibrary();

public:

	static int startLibrary(bool xmlconfig);

	static int startLibrary( int width , int height , int bpp , bool fullscreen, bool doublebuff=true ) ;
	
	static int processEngines();

	
	inline static CLibrary* getLibrary();	

	inline static CEngine* getActualEngine();

	static int addEngine(CEngine* engine,int priority);

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

	bool static orderEngine(CEngine*,CEngine*);

};

#define SINERROR "|-| No error"

string resource(const char* c);

#endif //#ifndef __APPLICATION_H__
