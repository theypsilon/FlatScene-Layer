#ifndef __FREEZEGAMEINTERFACE_H__
#define __FREEZEGAMEINTERFACE_H__

#include "Engine.h"
#include "definitions.h"
 
using namespace std;

class CFreezeGameInterface : public CEngine  
{
private:
	bool pushed;

	CEngine* previous;
public:

	CFreezeGameInterface(CMessageHandler* pmhParent=NULL);

	virtual ~CFreezeGameInterface();

	virtual int onInit();

	virtual int drawFrame();

	virtual int onExit();


	CEngine* setPrevious(CEngine*);

	
	static void onKeyFreeze(SDL_Event*);

	virtual void onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode);
	virtual void onKeyUp(SDLKey sym,SDLMod mod,Uint16 unicode);

	virtual int onMessage(Uint32 MsgID, MSGPARM Parm1, MSGPARM Parm2, MSGPARM Parm3, MSGPARM Parm4);


	static Uint32 MSGID_PreviousEngine;
};

#endif 