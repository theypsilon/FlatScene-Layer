// FreezeGameInterface.cpp: implementation of the CFreezeGameInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "FreezeGameInterface.h"
#include "FSLibrary.h"


Uint32 CFreezeGameInterface::MSGID_PreviousEngine=FSMessageHandler::getNextMSGID();

//constructor
CFreezeGameInterface::CFreezeGameInterface(FSMessageHandler * pmhParent) : FSMessageHandler(pmhParent), previous(nullptr) {
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

    return FSEngine::onInit();
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

    return FSEngine::onExit();

}

std::shared_ptr<FSEngine> CFreezeGameInterface::setPrevious(std::shared_ptr<FSEngine> ePrev) {

    std::shared_ptr<FSEngine> ret = previous;
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

void CFreezeGameInterface::onKeyUp(SDLKey sym,SDLMod mod,Uint16 unicode) {
    if (sym==SDLK_SPACE && pushed) {
        getParent()->SendMessage(FSLib.MSGID_RunEngine,(void*)previous.get());
        getParent()->SendMessage(FSLib.MSGID_KillEngine,(void*)shared_from_this().get());
    } else if (sym==SDLK_F1 && pushed) {
        previous->done = true;

        getParent()->SendMessage(FSLib.MSGID_KillEngine,(void*)shared_from_this().get());
        getParent()->SendMessage(FSLib.MSGID_ChangeEngine);

    }
}
