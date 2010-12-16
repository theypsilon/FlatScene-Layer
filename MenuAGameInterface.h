#ifndef __MENUAGAMEINTERFACE_H__
#define __MENUAGAMEINTERFACE_H__

#include "Engine.h"
#include "definitions.h"
#include "Point.h"
#include <vector>
#include <string>
 
using namespace std;
/*
	==CMenuAGameInterface==
	CEngine componEnt test.
*/
class CMenuAGameInterface : public CEngine  
{
private:
	bool pushed;

	Uint8 opcion;

	int file;
	vector<int> IdTexts;
	list<string> texts;

	CPoint dest;

	CEngine* previous;
public:
	//constructor
	CMenuAGameInterface(CMessageHandler* pmhParent=NULL);
	//destructor
	virtual ~CMenuAGameInterface();
	//initialization
	virtual int onInit();
	//idle behavior
	virtual int onIdle();
	//cleanup
	virtual int onExit();

	CEngine* setPrevious(CEngine*);
	
	static void onKeyMenu(SDL_Event*);

	virtual void onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode);
	virtual void onKeyUp(SDLKey sym,SDLMod mod,Uint16 unicode);
};

#endif 
