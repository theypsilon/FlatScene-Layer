//include guards
#ifndef __MESSAGEHANDLER_H__
#define __MESSAGEHANDLER_H__

//include sdl

#ifdef WIN32
#include <windows.h>
#endif

#include "GL/gl.h"			// Librería OpenGL32 
#include "GL/glu.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_opengl.h"
 
#include "definitions.h"
#include <list>
#include <vector>

typedef void* MSGPARM;

class CMessage {
private:
	Uint32 MsgID;
	MSGPARM Parm1;
	MSGPARM Parm2;

	CMessage(Uint32 MsgID=0,MSGPARM Parm1=NULL,MSGPARM Parm2=NULL);
	virtual ~CMessage();
	virtual Uint32 getMsgID();
	virtual MSGPARM getParm(int n);

	friend class CMessageHandler;
	friend class CLibrary;
};

typedef std::list<CMessage*> MessageCollection;

class CMessageHandler  {
private:
	CMessageHandler* m_pmhParent;

	static Uint32 s_NextMSGID;

	MessageCollection bufferMessages;

	friend class CLibrary;

#ifdef MENSAJES_MSGIDS
	static int textM0;
#endif
protected:
	virtual void pendingMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2);
	virtual int onMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2);

	virtual void OnaddChild(CMessageHandler* pmhChild);
	virtual void onRemoveChild(CMessageHandler* pmhChild);
public:
	CMessageHandler(CMessageHandler* pmhParent);
	virtual ~CMessageHandler();
	
	CMessageHandler* getParent();
	void setParent(CMessageHandler* pmhNewParent);	
	bool HasParent();
	
#ifdef MENSAJES_MSGIDS
	virtual int SendMessage(Uint32 MsgID,MSGPARM Parm1=NULL,MSGPARM Parm2=NULL);
#else
	int SendMessage(Uint32 MsgID,MSGPARM Parm1=NULL,MSGPARM Parm2=NULL);
#endif

	void readMessages();
	void clearAllMessages();
	
	//Cada MSGID debe usar esta función al definirse para garantizar la no repetición de identificadores.
	static Uint32 getNextMSGID(bool instant=true);
	
	//msgid: Para establecer jerarquías: Parm1=Parent, Parm2=Child
	static Uint32 MSGID_addChild;
	static Uint32 MSGID_RemoveChild;
	
};

#endif //#ifndef __MESSAGEHANDLER_H__
