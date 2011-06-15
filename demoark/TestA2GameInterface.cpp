// TestA2GameInterface.cpp: implementation of the CTestA2GameInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "TestA2GameInterface.h"
#include "FSLibrary.h"
#include "FSControlOutputText.h"

#include "EnemyPunto.h"
#include "EnemyNPC.h"

#include "FreezeGameInterface.h"
#include "MenuAGameInterface.h"

//constructor
CTestA2GameInterface::CTestA2GameInterface(CMessageHandler* pmhParent) : CTestAGameInterface(pmhParent)
{

}

//destructor
CTestA2GameInterface::~CTestA2GameInterface()
{

		readMessages();
}

//initialization
int  CTestA2GameInterface::onInit() {
	//initialize parent class
	if (CEngine::onInit() == FRACASO)
		return FRACASO;

	CScreen::clear();

#ifdef LOG_SISTEMA
	printf("\nTestA0 comienza.\n\n");
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

	activationIds.clear();
	activationIds.push_back("npc_normal_quiet");
	activationIds.push_back("npc_normal_walk");

	TiXmlDocument xmldoc("resources/config.xml");	// Cargamos el archivo de configuración.
	if (!xmldoc.LoadFile()) {	 CLibrary::Error("resources/config.xml",TE_fileExists); }

	TiXmlHandle input(xmldoc.FirstChildElement("System"));
	int cantNPC=0;

	if (input.FirstChildElement("demonpc").ToElement() && input.FirstChildElement("demonpc").ToElement()->Attribute("cant"))
		cantNPC = atoi(input.FirstChildElement("demonpc").ToElement()->Attribute("cant"));

	if (cantNPC < 0 || cantNPC > 100000)
		cantNPC = 0;

	CMap* mapDemo; 
	if (input.FirstChildElement("demonpc").ToElement() && input.FirstChildElement("demonpc").ToElement()->Attribute("initial-map"))
		mapDemo = (CMap*) CMultiverse.add(new CMap(input.FirstChildElement("demonpc").ToElement()->Attribute("initial-map")));
	else
		mapDemo = (CMap*) CMultiverse.add(new CMap("mapa2"));
	mapDemo->load();

	cantNPC = 3000;

	if (cantNPC >= 1) {
		CEnemy* ene = CEnemy::Factory("E0",this);
		enemy.push_back(ene);
		ene->init(activationIds,100+rand()%(mapDemo->getW()*mapDemo->getTileW()-200),100+rand()%(mapDemo->getH()*mapDemo->getTileH()-200),0);
		srand(rand());
		mapDemo->incActor(ene);
/*
		for (int i=0;i<100;i++) {
			for (int j=0;j<100;j++) {
				CEnemy* eneClon = (CEnemy*) ene->clone();
				enemy.push_back(eneClon);
				eneClon->m_Scrollxy.set(mapDemo->getW()*mapDemo->getTileW()/2-50+i,mapDemo->getH()*mapDemo->getTileH()/2-50+j,0);
				srand(rand());
				mapDemo->incActor(eneClon);
			}
		}*/

		for (int i=1;i<cantNPC;i++) {
			CEnemy* eneClon = (CEnemy*) ene->clone();
			enemy.push_back(eneClon);
			eneClon->m_Scrollxy.set(100+rand()%(mapDemo->getW()*mapDemo->getTileW()-200),100+rand()%(mapDemo->getH()*mapDemo->getTileH()-200),0);
			srand(rand());
			mapDemo->incActor(eneClon);
		}
	}

	player[0]->m_Scrollxy.set(mapDemo->getW()*mapDemo->getTileW()/2,mapDemo->getH()*mapDemo->getTileH()/2,0);
	player[1]->m_Scrollxy.set(mapDemo->getW()*mapDemo->getTileW()/2,mapDemo->getH()*mapDemo->getTileH()/2-50,0);
	mapDemo->incActor((CActor*)player[0]);
	mapDemo->incActor((CActor*)player[1]);

	cams.push_back(new CScrollCamera((CActor*)player[0],new CRectangle(0,0,RESOLUCION_X,RESOLUCION_Y),NULL,0.35));

	Write.color(Write.inBox("resources/texts0",0),1.0,1.0,1.0,0.7,TCTB_BOX,true);

	Write.line(0,130,5,"ESC para Menu.");

	Write.line(0,230,5,"Teclas:");
	Write.line(0,230,20,"cursores : andar");
	Write.line(0,230,35,"a : atacar");
	Write.line(0,230,50,"F1 : siguiente test");

	Write.line(0,130,230,"Test 2");

	return EXITO;
}

//initialization
int  CTestA2GameInterface::onIdle() {
	//initialize parent class
	return CTestAGameInterface::onIdle();
}

//on cleanup
int CTestA2GameInterface::onExit()
{
	
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
	printf("\nTestA0 termina.\n\n");
#endif

	return CTestAGameInterface::onExit();
}

void CTestA2GameInterface::onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode) {
	if (sym==SDLK_ESCAPE) {

		CMenuAGameInterface* men = new CMenuAGameInterface(CLibrary::getLibrary());
		men->setEventHandler(SDL_KEYDOWN,&CMenuAGameInterface::onKeyMenu);
		men->setEventHandler(SDL_KEYUP,&CMenuAGameInterface::onKeyMenu);

		men->setPrevious(this);

		CLibrary::getLibrary()->SendMessage(CLibrary::MSGID_RunEngine, (MSGPARM)men);
	} else if (sym==SDLK_SPACE) {

		CFreezeGameInterface* fgi = new CFreezeGameInterface(CLibrary::getLibrary());
		fgi->setEventHandler(SDL_KEYDOWN,&CFreezeGameInterface::onKeyFreeze);
		fgi->setEventHandler(SDL_KEYUP,&CFreezeGameInterface::onKeyFreeze);

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
	} else if (sym==SDLK_TAB) {
		if (cams[0]->Target() == player[0])
			cams[0]->setTarget(player[1]);
		else
			cams[0]->setTarget(player[0]);
	} else if (sym==SDLK_LCTRL) {
		srand(rand());
		int cont = player.size() + enemy.size();
		int v = rand()%cont;
		if ( v < player.size() )
			cams[0]->setTarget(player[v]);
		else {
			v -= player.size();
			if (v==0)
				cams[0]->setTarget(enemy.front());
			else {
				for (list<CEnemy*>::iterator it=enemy.begin(),et=enemy.end();it!=et;++it) {
					v--;
					if (v<=0) {
						cams[0]->setTarget(*it);
						break;
					}
				}
			}
		}
	} 
	CTestAGameInterface::onKeyDown(sym,mod,unicode);
}
