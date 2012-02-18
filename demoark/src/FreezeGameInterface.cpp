// FreezeGameInterface.cpp: implementation of the CFreezeGameInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "FreezeGameInterface.h"
#include "FSLibrary.h"

CFreezeGameInterface::CFreezeGameInterface() 
: previous(nullptr), pushed(false) {}

CFreezeGameInterface::~CFreezeGameInterface() {}

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

FSEngine* CFreezeGameInterface::setPrevious(FSEngine* ePrev) {

    auto ret = previous;
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
        FSLib.exit();
    } else if (sym==SDLK_F1 && pushed) {
        previous->done = true;
        FSLib.exit();

    }
}
