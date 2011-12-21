// FreezeGameInterface.cpp: implementation of the CTestBGameInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "TestBGameInterface.h"
#include "FSLibrary.h"
#include "FSScreen.h"

//constructor
CTestBGameInterface::CTestBGameInterface(CMessageHandler * pmhParent) : CEngine(pmhParent), actZone(0)
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
	if (CEngine::onInit() == FRACASO)
		return FRACASO;

	CScreen::clear();

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
	CScreen::clear();

	CSprite* spt = anim->paso_a_paso_spt();
	
	CPoint paux(100,100);
	
	spt->put(paux);

	if (spt->size() > actZone) {

		CScreen::pushMatrix();

		CPoint ptDst(*spt->getCenter());
		ptDst.X()+=100;
		ptDst.Y()+=100;

		CScreen::translate((float)ptDst.X(),(float)ptDst.Y(),0);  

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

	return CEngine::onExit();
}

void CTestBGameInterface::onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode) {
	if (sym==SDLK_TAB && !pushed) {
		pushed=true;
	}
}

void CTestBGameInterface::onKeyUp(SDLKey sym,SDLMod mod,Uint16 unicode) {
	if (sym==SDLK_TAB && pushed) {
		getParent()->SendMessage(CLibrary::MSGID_ChangeEngine,(void*)false);
		pushed=false;
	}
}
