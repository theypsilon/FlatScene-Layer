// TestA1GameInterface.cpp: implementation of the CTestA1GameInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "TestA1GameInterface.h"
#include "FSLibrary.h"
#include "FreezeGameInterface.h"
#include "MenuAGameInterface.h"

//constructor
CTestA1GameInterface::CTestA1GameInterface(CMessageHandler* pmhParent) : CTestAGameInterface(pmhParent)
{

}

//destructor
CTestA1GameInterface::~CTestA1GameInterface()
{

		readMessages();
}

int CTestA1GameInterface::drawFrame() {

	CScreen::projectionMode(TRP_ORTHO);

	for (int i=0;i<cams.size();i++) {

		cams[i]->locateRenderScene(cams[i]->getArea()->X()*2,cams[i]->getArea()->Y()*2,cams[i]->getArea()->W()*2,cams[i]->getArea()->H()*2);
		cams[i]->render();

	}

	Write.render();

	return EXITO;

}

//initialization
int CTestA1GameInterface::onInit() {
	//initialize parent class
	if (CEngine::onInit() == FRACASO)
		return FRACASO;

	CScreen::clear();

#ifdef LOG_SISTEMA
	printf("\nTestA1 comienza.\n\n");
#endif

	//Inicializando array de correspondencia de teclas.
#ifdef EVENTOS_RAPIDO
	for (int i=0;i<NUM_SDLKEY;i++)
		erasePlayerKeyAlias(i);
#endif

	list<string> activationIds;
	activationIds.push_back("pj_normal_quiet");
	activationIds.push_back("pj_normal_walk");
	activationIds.push_back("pj_normal_run");
	activationIds.push_back("pj_normal_ba");
	activationIds.push_back("pj_normal_sa");
	activationIds.push_back("pj_normal_pba");
	activationIds.push_back("pj_normal_osa");
	activationIds.push_back("pj_normal_aba");
	activationIds.push_back("pj_normal_asa");
	activationIds.push_back("pj_normal_alsa");

	player.push_back(new CPlayer((CMessageHandler*)this));	
	player[0]->eventChange=false;

	
	player[0]->init(activationIds,0);
	
	player.push_back(new CPlayer((CMessageHandler*)this)); 
	player[1]->eventChange=false;
	player[1]->init(activationIds,1);
	player[1]->m_Scrollxy.set(270,100,0);

	CMultiverse.add(new CMap("mapa1"))->incActor((CActor*)player[0]);
	//CMultiverse.get(0)->incActor((CActor*)player[1]);
	CMultiverse.add(new CMap("mapa1"))->incActor((CActor*)player[1]);

	cams.push_back(new CScrollCamera((CActor*)player[0],new CRectangle(0,0,RESOLUCION_X/2,RESOLUCION_Y/2),NULL,0.75));
	cams.push_back(new CScrollCamera((CActor*)player[1],new CRectangle(0,RESOLUCION_Y/2,RESOLUCION_X/2,RESOLUCION_Y/2),NULL,0.35));
	cams.push_back(new CScrollCamera((CActor*)player[1],new CRectangle(RESOLUCION_X/2,0,RESOLUCION_X/2,RESOLUCION_Y/2),new CPoint(80,100)));
	cams.push_back(new CScrollCamera((CActor*)player[0],new CRectangle(RESOLUCION_X/2,RESOLUCION_Y/2,RESOLUCION_X/2,RESOLUCION_Y/2)));


	Write.line(0,460,10,"Teclas:");
	Write.line(0,460,40,"cursores : andar");
	Write.line(0,460,70,"a : atacar");
	Write.line(0,460,100,"F1 : siguiente test");

	Write.line(0,260,450,"Test 1");

	return EXITO;
}

int CTestA1GameInterface::onExit() {
	
	CCamera* c ;
	for ( CameraCollection::iterator it ; !cams.empty ( ); )
	{
		it = cams.begin ( ) ;
		c = *it ;
		cams.erase ( it ) ;
		delete c ;
	}

	player.clear();
	enemy.clear();

#ifdef LOG_SISTEMA
	printf("\nTestA1 termina.\n\n");
#endif

	return CTestAGameInterface::onExit();
}

void CTestA1GameInterface::onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode) {
	if (sym==SDLK_ESCAPE) {

		CMenuAGameInterface* men = new CMenuAGameInterface(CLibrary::getLibrary());
		men->setEventHandler(SDL_KEYDOWN,CMenuAGameInterface::onKeyMenu);
		men->setEventHandler(SDL_KEYUP,CMenuAGameInterface::onKeyMenu);

		men->setPrevious(this);

		CLibrary::getLibrary()->SendMessage(CLibrary::MSGID_RunEngine, (MSGPARM)men);
	} else if (sym==SDLK_SPACE) {
		CFreezeGameInterface* fgi = new CFreezeGameInterface(CLibrary::getLibrary());
		fgi->setEventHandler(SDL_KEYDOWN,CFreezeGameInterface::onKeyFreeze);
		fgi->setEventHandler(SDL_KEYUP,CFreezeGameInterface::onKeyFreeze);

		fgi->setPrevious(this);

		CLibrary::getLibrary()->SendMessage(CLibrary::MSGID_RunEngine, (MSGPARM)fgi);
	} else if (sym==SDLK_DELETE) {
		getParent()->SendMessage(CLibrary::MSGID_Restart);
	} else if (sym==SDLK_F1) {
		getParent()->SendMessage(CLibrary::MSGID_ChangeEngine);
	} else if (sym==SDLK_F2) {
		getParent()->SendMessage(CLibrary::MSGID_ReloadEngine,(MSGPARM)this);
	} else if (sym==SDLK_F3) {
		deselect();
		CScreen::ToggleFullscreen();
		loop();
	}
	CTestAGameInterface::onKeyDown(sym,mod,unicode);
}
