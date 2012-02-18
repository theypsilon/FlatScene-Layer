#ifndef __TESTA3GAMEINTERFACE_H__
#define __TESTA3GAMEINTERFACE_H__

#include "TestAGameInterface.h"

#define TestIdA3 "TestA_2players"


class CTestA3GameInterface : public CTestAGameInterface {
private:
    int score1,score2;
public:

    CTestA3GameInterface();

    virtual ~CTestA3GameInterface();

    int onInit();

    int onExit();

    int drawFrame();

    void killEnemy(FSActor* victim, FSActor* murder, FSUniverse* map);

    void onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode);
};

#endif 