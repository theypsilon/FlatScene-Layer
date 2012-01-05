// FreezeGameInterface.cpp: implementation of the CTestBGameInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "TestBGameInterface.h"
#include "FSLibrary.h"
#include "FSScreen.h"

//constructor
CTestBGameInterface::CTestBGameInterface(FSMessageHandler * pmhParent) : FSEngine(pmhParent), actZone(0)
{
	pushed=false;
}

//destructor
CTestBGameInterface::~CTestBGameInterface()
{

}

//initialization
int CTestBGameInterface::onInit() {
#ifdef LOG_SISTEMA
	printf("\nTestB comienza.\n\n");
#endif
	if (FSEngine::onInit() == FRACASO)
		return FRACASO;

	FSScreen::clear();

/*	int num=1;
	int* sequence = alloc(int,num);
	for (int i=0;i<num;i++)
		sequence[i]=i;
	
	CImg.add("Character");

	anim = new PAnimation(num,sequence,0);*/

	Chrono.setInterval(64);

	return EXITO;
}

//idle. Main loop.
int CTestBGameInterface::onIdle()
{
	FSScreen::clear();

	FSSprite* spt = anim->paso_a_paso_spt();
	
	FSPoint paux(100,100);
	
	spt->put(paux);

	if (spt->size() > actZone) {

		FSScreen::pushMatrix();

		FSPoint ptDst(*spt->getCenter());
		ptDst.x+=100;
		ptDst.y+=100;

		FSScreen::translate((float)ptDst.x,(float)ptDst.y,0);  

	}

	return EXITO;
	
}

//on cleanup
int CTestBGameInterface::onExit()
{
	delete anim;

	CImg.remove(CImg.search("resources/Character"));

#ifdef LOG_SISTEMA
	printf("\nTestB termina.\n\n");
#endif

	return FSEngine::onExit();
}

void CTestBGameInterface::onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode) {
	if (sym==SDLK_TAB && !pushed) {
		pushed=true;
	}
}

void CTestBGameInterface::onKeyUp(SDLKey sym,SDLMod mod,Uint16 unicode) {
	if (sym==SDLK_TAB && pushed) {
		getParent()->SendMessage(FSLibrary::MSGID_ChangeEngine,(void*)false);
		pushed=false;
	}
}
