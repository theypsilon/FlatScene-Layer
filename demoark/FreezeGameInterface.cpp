// FreezeGameInterface.cpp: implementation of the CFreezeGameInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "FreezeGameInterface.h"
#include "FSLibrary.h"


Uint32 CFreezeGameInterface::MSGID_PreviousEngine=CMessageHandler::getNextMSGID();

//constructor
CFreezeGameInterface::CFreezeGameInterface(CMessageHandler* pmhParent) : CEngine(pmhParent), previous(NULL) {
	pushed=false;
}

//destructor
CFreezeGameInterface::~CFreezeGameInterface() {

}

//initialization
int CFreezeGameInterface::onInit() {

#ifdef LOG_SISTEMA
	printf("\nFreeze comienza.\n\n");
#endif

	return CEngine::onInit();
}

//idle. Main loop.
int CFreezeGameInterface::drawFrame() {
	
	if (previous)
		return previous->drawFrame();

	return EXITO;
	
}

//on cleanup
int CFreezeGameInterface::onExit() {

#ifdef LOG_SISTEMA
	printf("\nFreeze termina.\n\n");
#endif

	return CEngine::onExit();

}

CEngine* CFreezeGameInterface::setPrevious(CEngine* ePrev) {

	CEngine* ret = previous;
	previous = ePrev;
	return ret;

}

void CFreezeGameInterface::onKeyFreeze(SDL_Event* event) {
	if (event->type == SDL_KEYDOWN)
		onKeyDown(event->key.keysym.sym,event->key.keysym.mod,event->key.keysym.unicode);
	else
		onKeyUp(event->key.keysym.sym,event->key.keysym.mod,event->key.keysym.unicode);
}

void CFreezeGameInterface::onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode) {
	if ((sym==SDLK_SPACE || sym==SDLK_F1)&& !pushed) {
		pushed=true;
	}
}

int CFreezeGameInterface::onMessage(Uint32 MsgID, MSGPARM Parm1, MSGPARM Parm2, MSGPARM Parm3, MSGPARM Parm4) {
	if (MsgID == CFreezeGameInterface::MSGID_PreviousEngine) {
		previous = (CEngine*) Parm1;
		return EXITO;
	} else
		return CEngine::onMessage(MsgID,Parm1,Parm2);
}

void CFreezeGameInterface::onKeyUp(SDLKey sym,SDLMod mod,Uint16 unicode) {
	if (sym==SDLK_SPACE && pushed) {
		getParent()->SendMessage(CLibrary::MSGID_RunEngine,(void*)previous);
		getParent()->SendMessage(CLibrary::MSGID_KillEngine,(void*)this);
	} else if (sym==SDLK_F1 && pushed) {
		previous->done = true;

		getParent()->SendMessage(CLibrary::MSGID_KillEngine,(void*)this);
		getParent()->SendMessage(CLibrary::MSGID_ChangeEngine);

	}
}
