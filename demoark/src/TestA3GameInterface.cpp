#include "TestA3GameInterface.h"
#include "FSLibrary.h"
#include "FSWriter.h"
#include "FreezeGameInterface.h"
#include "MenuAGameInterface.h"

#include "EnemyPunto.h"
#include "EnemyNPC.h"

//constructor
CTestA3GameInterface::CTestA3GameInterface(FSMessageHandler * pmhParent) : CTestAGameInterface(pmhParent)
{
	score1=score2=0;
}

//destructor
CTestA3GameInterface::~CTestA3GameInterface()
{

		readMessages();
}

//initialization
int CTestA3GameInterface::onInit() {
	//initialize parent class
	if (FSEngine::onInit() == FRACASO)
		return FRACASO;

	FSScreen::clear();

#ifdef LOG_SISTEMA
	printf("\nTestA3 comienza.\n\n");
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



	player.push_back(new CPlayer(this));	
	player[0]->eventChange=false;
	
	player[0]->init(activationIds,0);

	player.push_back(new CPlayer(this));	
	player[1]->eventChange=false;
	
	player[1]->init(activationIds,1);

	activationIds.clear();
	activationIds.push_back("npc_normal_quiet");
	activationIds.push_back("npc_normal_walk");

	CMap* mapDemo = (CMap*) Cosmos.add(new CMap("mapextmur"));
	mapDemo->load();

	player[0]->m_Scrollxy.set(mapDemo->getW()*mapDemo->getTileW()/2,mapDemo->getH()*mapDemo->getTileH()/2,0);
	player[1]->m_Scrollxy.set(mapDemo->getW()*mapDemo->getTileW()/2,mapDemo->getH()*mapDemo->getTileH()/2-50,0);
	mapDemo->incActor((FSActor*)player[0]);
	mapDemo->incActor((FSActor*)player[1]);

	CEnemy* ene = CEnemy::Factory("E0",this);
	enemy.push_back(ene);
	ene->init(activationIds,100+rand()%(mapDemo->getW()*mapDemo->getTileW()-200),100+rand()%(mapDemo->getH()*mapDemo->getTileH()-200),0);
	srand(rand());
	mapDemo->incActor(ene);
	while (ene->collisionMap(0,0)) {
		mapDemo->decActor(ene);
		ene->m_Scrollxy.set(100+rand()%(mapDemo->getW()*mapDemo->getTileW()-200),100+rand()%(mapDemo->getH()*mapDemo->getTileH()-200),0);
		srand(rand());
		mapDemo->incActor(ene);
	}

	for (int i=1;i<50;i++) {
		CEnemy* eneClon = (CEnemy*) ene->clone();
		enemy.push_back(eneClon);
		eneClon->m_Scrollxy.set(100+rand()%(mapDemo->getW()*mapDemo->getTileW()-200),100+rand()%(mapDemo->getH()*mapDemo->getTileH()-200),0);
		srand(rand());
		mapDemo->incActor(eneClon);
		while (eneClon->collisionMap(0,0)) {
			mapDemo->decActor(eneClon);
			eneClon->m_Scrollxy.set(100+rand()%(mapDemo->getW()*mapDemo->getTileW()-200),100+rand()%(mapDemo->getH()*mapDemo->getTileH()-200),0);
			srand(rand());
			mapDemo->incActor(eneClon);
		}

	}

	score1=score2=0;

	cams.push_back(new CScrollCamera((FSActor*)player[0],new FSRectangle(0,0,RESOLUCION_X/2,RESOLUCION_Y),NULL,0.35));
	cams.push_back(new CScrollCamera((FSActor*)player[1],new FSRectangle(RESOLUCION_X/2,0,RESOLUCION_X/2,RESOLUCION_Y),NULL,0));

	Write.line(0,260,10,"ESC para Menu.");

	//Write.line(0,230,5,"Teclas:");
	//Write.line(0,230,20,"cursores : andar p1");
	//Write.line(0,230,35,"a : atacar p1");
	//Write.line(0,230,50,"jkli : andar p2");
	//Write.line(0,230,65,"num0 : atacar p2");
	//Write.line(0,230,80,"F1 : siguiente test");

	//Write.line(0,130,230,"Test 3");

	return EXITO;
}

//on cleanup
int CTestA3GameInterface::onExit()
{
	
	FSCamera* c ;
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
	printf("\nTestA3 termina.\n\n");
#endif

	return CTestAGameInterface::onExit();
}

int CTestA3GameInterface::drawFrame() {

	int l1 = Write.line(0,120,430,"Score: %d",score1);
	int l2 = Write.line(0,440,430,"Score: %d",score2);

	Write.erase(l1,true);
	Write.erase(l2,true);

	return CTestAGameInterface::drawFrame();
}

void CTestA3GameInterface::onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode) {
	if (sym==SDLK_ESCAPE) {

		CMenuAGameInterface* men = new CMenuAGameInterface(FSLibrary::getLibrary());
		men->setEventHandler(SDL_KEYDOWN,&CMenuAGameInterface::onKeyMenu);
		men->setEventHandler(SDL_KEYUP,&CMenuAGameInterface::onKeyMenu);

		men->setPrevious(this);

		FSLibrary::getLibrary()->SendMessage(FSLibrary::MSGID_RunEngine, (MSGPARM)men);
	} else if (sym==SDLK_SPACE) {
		CFreezeGameInterface* fgi = new CFreezeGameInterface(FSLibrary::getLibrary());
		fgi->setEventHandler(SDL_KEYDOWN,&CFreezeGameInterface::onKeyFreeze);
		fgi->setEventHandler(SDL_KEYUP,&CFreezeGameInterface::onKeyFreeze);

		fgi->setPrevious(this);

		FSLibrary::getLibrary()->SendMessage(FSLibrary::MSGID_RunEngine, (MSGPARM)fgi);
	} else if (sym==SDLK_DELETE) {
		getParent()->SendMessage(FSLibrary::MSGID_Restart);
	} else if (sym==SDLK_F1) {
		getParent()->SendMessage(FSLibrary::MSGID_ChangeEngine);
	} else if (sym==SDLK_F2) {
		getParent()->SendMessage(FSLibrary::MSGID_ReloadEngine,(MSGPARM)this);
	} else if (sym==SDLK_F3) {
		deselect();
		FSScreen::ToggleFullscreen();
		loop();
	}
	CTestAGameInterface::onKeyDown(sym,mod,unicode);
}

void CTestA3GameInterface::pendingMessage(Uint32 MsgID, MSGPARM Parm1, MSGPARM Parm2) {
	
	if (MsgID==CTestAGameInterface::MSGID_KillEnemy) {

		void** parm = (void**) Parm2;

		CPlayer* murder = (CPlayer*) parm[1];
		if (murder) {
			if (player[0] == murder) {
				score1++;
			} else if (player[1] == murder) {
				score2++;
			}
		}
		
	}

	CTestAGameInterface::pendingMessage(MsgID,Parm1,Parm2);
}
