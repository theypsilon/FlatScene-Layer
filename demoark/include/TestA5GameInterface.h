#ifndef __TESTA5GAMEINTERFACE_H__
#define __TESTA5GAMEINTERFACE_H__

#include "TestAGameInterface.h"

#define TestIdA0 "TestA_base"


class CTestA5GameInterface : public CTestAGameInterface {
public:

    CTestA5GameInterface();

    virtual ~CTestA5GameInterface();

    int onInit();

    int drawFrame();

    int onExit();

    void onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode);
};

#endif 