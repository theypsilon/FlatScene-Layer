//include guards
#ifndef __MESSAGEHANDLER_H__
#define __MESSAGEHANDLER_H__

//include sdl

#ifdef WIN32
#include <windows.h>
#endif

#include "GL/gl.h"    		// Librería OpenGL32 
#include "GL/glu.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_opengl.h"
 
#include "FSdefinitions.h"
#include <list>
#include <vector>

typedef void* MSGPARM;

class FSMessageHandler  {
private:
    class FSMessage {
    private:
    	Uint32 MsgID;
    	MSGPARM Parm1;
    	MSGPARM Parm2;

    	FSMessage(Uint32 MsgID=0,MSGPARM Parm1=NULL,MSGPARM Parm2=NULL);
    	virtual ~FSMessage();
    	virtual Uint32 getMsgID();
    	virtual MSGPARM getParm(int n);

    	friend class FSMessageHandler;
    	friend class FSLibrary;
    };

    typedef std::list<FSMessage*> MessageCollection;

    FSMessageHandler * m_pmhParent;

    static Uint32 s_NextMSGID;

    MessageCollection bufferMessages;

    friend class FSLibrary;

#ifdef MENSAJES_MSGIDS
    static int textM0;
#endif
protected:
    virtual void pendingMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2);
    virtual int onMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2);

    virtual void OnaddChild(FSMessageHandler* pmhChild);
    virtual void onRemoveChild(FSMessageHandler* pmhChild);
public:
    FSMessageHandler(FSMessageHandler * pmhParent);
    virtual ~FSMessageHandler();
    
    FSMessageHandler * getParent();
    void setParent(FSMessageHandler * pmhNewParent);	
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
