#ifndef __FREEZEGAMEINTERFACE_H__
#define __FREEZEGAMEINTERFACE_H__

#include "FSEngine.h"
#include "FSMessageHandler.h"

#include "FSdefinitions.h"

class CFreezeGameInterface : public FSEngine, public FSMessageHandler
{
private:
    bool pushed;

    std::shared_ptr<FSEngine> previous;
public:

    CFreezeGameInterface(FSMessageHandler * pmhParent=NULL);

    virtual ~CFreezeGameInterface();

    virtual int onInit();

    virtual int drawFrame();

    virtual int onExit();


    std::shared_ptr<FSEngine> setPrevious(std::shared_ptr<FSEngine>);

    
    void onKeyFreeze(SDL_Event*);

    virtual void onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode);
    virtual void onKeyUp(SDLKey sym,SDLMod mod,Uint16 unicode);


    static Uint32 MSGID_PreviousEngine;
};

#endif 