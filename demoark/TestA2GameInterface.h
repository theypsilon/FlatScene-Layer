#ifndef __TESTA2GAMEINTERFACE_H__
#define __TESTA2GAMEINTERFACE_H__

#include "TestAGameInterface.h"

#define TestIdA2 "TestA_animacion"
using namespace std;


class CTestA2GameInterface : public CTestAGameInterface {
private:
	int actZone;

	CPlayer* principal;
public:

	CTestA2GameInterface(CMessageHandler * pmhParent=NULL);

	virtual ~CTestA2GameInterface();

	int onInit();

	int onIdle();
	int onExit();

	void onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode);
};

#endif //#ifndef __TESTEVENTHANDLER_H__