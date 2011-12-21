#ifndef __TESTA3GAMEINTERFACE_H__
#define __TESTA3GAMEINTERFACE_H__

#include "TestAGameInterface.h"

#define TestIdA3 "TestA_2players"
using namespace std;


class CTestA3GameInterface : public CTestAGameInterface {
private:
	int score1,score2;
public:

	CTestA3GameInterface(CMessageHandler * pmhParent=NULL);

	virtual ~CTestA3GameInterface();

	int onInit();

	int onExit();

	int drawFrame();

	void onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode);

	void pendingMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2);
};

#endif 