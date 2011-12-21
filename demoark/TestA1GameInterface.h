#ifndef __TESTA1GAMEINTERFACE_H__
#define __TESTA1GAMEINTERFACE_H__

#include "TestAGameInterface.h"

#define TestIdA1 "TestA_camaras"
using namespace std;

class CTestA1GameInterface : public CTestAGameInterface {
public:

	CTestA1GameInterface(CMessageHandler * pmhParent=NULL);

	virtual ~CTestA1GameInterface();

	int onInit();

	int onExit();

	int drawFrame();

	void onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode);
};

#endif //#ifndef __TESTEVENTHANDLER_H__