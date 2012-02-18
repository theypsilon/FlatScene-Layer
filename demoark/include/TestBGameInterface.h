#ifndef __TESTBGAMEINTERFACE_H__
#define __TESTBGAMEINTERFACE_H__

#include "FSEngine.h"
#include "FSMessageHandler.h"

#include "FSdefinitions.h"
#include "PAnimation.h"

#define TestIdB "TestB"
 

class CTestBGameInterface : public FSEngine {
private:
    bool pushed;

    PAnimation* anim;

    int actZone;
public:

    CTestBGameInterface();

    virtual ~CTestBGameInterface();

    virtual int onInit();

    virtual int onIdle();

    virtual int onExit();

    virtual void onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode);
    virtual void onKeyUp(SDLKey sym,SDLMod mod,Uint16 unicode);
};

#endif 
