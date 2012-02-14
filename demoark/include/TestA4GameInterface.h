#ifndef __TESTA4GAMEINTERFACE_H__
#define __TESTA4GAMEINTERFACE_H__

#include "TestAGameInterface.h"

#define TestIdA0 "TestA_base"
using namespace std;


class CTestA4GameInterface : public CTestAGameInterface {
public:

    CTestA4GameInterface(FSMessageHandler * pmhParent=NULL);

    virtual ~CTestA4GameInterface();

    int onInit();

    int onExit();

    int drawFrame();

    void onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode);
};

#endif 