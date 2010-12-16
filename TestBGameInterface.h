#ifndef __TESTBGAMEINTERFACE_H__
#define __TESTBGAMEINTERFACE_H__

#include "Engine.h"
#include "definitions.h"
#include "PAnimation.h"

#define TestIdB "TestB"
 
using namespace std;
/*
	==CTestBGameInterface==
	CEngine componEnt test.
*/
class CTestBGameInterface : public CEngine  
{
private:
	bool pushed;

	PAnimation* anim;

	int actZone;
public:
	//constructor
	CTestBGameInterface(CMessageHandler* pmhParent=NULL);
	//destructor
	virtual ~CTestBGameInterface();
	//initialization
	virtual int onInit();
	//idle behavior
	virtual int onIdle();
	//cleanup
	virtual int onExit();
	//mouse move
	virtual void onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode);
	virtual void onKeyUp(SDLKey sym,SDLMod mod,Uint16 unicode);
};

#endif 