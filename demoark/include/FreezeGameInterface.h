#ifndef __FREEZEGAMEINTERFACE_H__
#define __FREEZEGAMEINTERFACE_H__

#include "FSEngine.h"
#include "FSMessageHandler.h"

#include "FSdefinitions.h"

class CFreezeGameInterface : public FSEngine {
private:
    bool pushed;

    FSEngine* previous;
public:

    CFreezeGameInterface();

    virtual ~CFreezeGameInterface();

    virtual int onInit();

    virtual int drawFrame();

    virtual int onExit();


    FSEngine* setPrevious(FSEngine*);

    
    void onKeyFreeze(SDL_Event*);

    virtual void onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode);
    virtual void onKeyUp(SDLKey sym,SDLMod mod,Uint16 unicode);
};

#endif 