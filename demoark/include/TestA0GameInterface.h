#ifndef __TESTA0GAMEINTERFACE_H__
#define __TESTA0GAMEINTERFACE_H__

#include "TestAGameInterface.h"

#define TestIdA0 "TestA_base"


class CTestA0GameInterface : public CTestAGameInterface {
private:

    bool pulsado;

public:

    CTestA0GameInterface(FSMessageHandler * pmhParent=NULL);

    virtual ~CTestA0GameInterface();

    int onInit();

    int onIdle();

    int onExit();

    void onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode);
    void onKeyUp(SDLKey sym,SDLMod mod,Uint16 unicode);
};

#endif 