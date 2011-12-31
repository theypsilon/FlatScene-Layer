#ifndef __FREEZEGAMEINTERFACE_H__
#define __FREEZEGAMEINTERFACE_H__

#include "FSEngine.h"
#include "FSdefinitions.h"
 
using namespace std;

class CFreezeGameInterface : public FSEngine  
{
private:
	bool pushed;

	FSEngine* previous;
public:

	CFreezeGameInterface(FSMessageHandler * pmhParent=NULL);

	virtual ~CFreezeGameInterface();

	virtual int onInit();

	virtual int drawFrame();

	virtual int onExit();


	FSEngine* setPrevious(FSEngine*);

	
	void onKeyFreeze(SDL_Event*);

	virtual void onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode);
	virtual void onKeyUp(SDLKey sym,SDLMod mod,Uint16 unicode);

	virtual int onMessage(Uint32 MsgID, MSGPARM Parm1, MSGPARM Parm2, MSGPARM Parm3, MSGPARM Parm4);


	static Uint32 MSGID_PreviousEngine;
};

#endif 