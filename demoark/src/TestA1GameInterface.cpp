// TestA1GameInterface.cpp: implementation of the CTestA1GameInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "TestA1GameInterface.h"
#include "FSLibrary.h"
#include "FreezeGameInterface.h"
#include "MenuAGameInterface.h"

//constructor
CTestA1GameInterface::CTestA1GameInterface(FSMessageHandler * pmhParent) : CTestAGameInterface(pmhParent)
{

}

//destructor
CTestA1GameInterface::~CTestA1GameInterface()
{

        readMessages();
}

int CTestA1GameInterface::drawFrame() {

    FSDraw.projectionMode(TRP_ORTHO);

    for (int i=0;i<cams.size();i++) {

        cams[i]->locateRenderScene(cams[i]->getArea()->x*2,cams[i]->getArea()->y*2,cams[i]->getArea()->w*2,cams[i]->getArea()->h*2);
        cams[i]->render();

    }

    Write.render();

    return EXITO;

}

//initialization
int CTestA1GameInterface::onInit() {
    //initialize parent class
    if (FSEngine::onInit() == FRACASO)
        return FRACASO;

    FSDraw.clear();

#ifdef LOG_SISTEMA
    printf("\nTestA1 comienza.\n\n");
#endif

    //Inicializando array de correspondencia de teclas.
#ifdef EVENTOS_RAPIDO
    for (int i=0;i<NUM_SDLKEY;i++)
        erasePlayerKeyAlias(i);
#endif

    std::list<std::string> activationIds;
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
    player[1]->m_Scrollxy.set(270,100,0);

    Cosmos.add(new CMap("mapa1"))->incActor((FSActor*)player[0]);
    //Cosmos.get(0)->incActor((CActor*)player[1]);
    Cosmos.add(new CMap("mapa1"))->incActor((FSActor*)player[1]);

    cams.push_back(new CScrollCamera((FSActor*)player[0],new FSRectangle(0,0,RESOLUCION_X/2,RESOLUCION_Y/2),NULL,0.75));
    cams.push_back(new CScrollCamera((FSActor*)player[1],new FSRectangle(0,RESOLUCION_Y/2,RESOLUCION_X/2,RESOLUCION_Y/2),NULL,0.35));
    cams.push_back(new CScrollCamera((FSActor*)player[1],new FSRectangle(RESOLUCION_X/2,0,RESOLUCION_X/2,RESOLUCION_Y/2),new FSPoint(80,100)));
    cams.push_back(new CScrollCamera((FSActor*)player[0],new FSRectangle(RESOLUCION_X/2,RESOLUCION_Y/2,RESOLUCION_X/2,RESOLUCION_Y/2)));


    Write.line(0,460,10,"Teclas:");
    Write.line(0,460,40,"cursores : andar");
    Write.line(0,460,70,"a : atacar");
    Write.line(0,460,100,"F1 : siguiente test");

    Write.line(0,260,450,"Test 1");

    return EXITO;
}

int CTestA1GameInterface::onExit() {
    
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
    printf("\nTestA1 termina.\n\n");
#endif

    return CTestAGameInterface::onExit();
}

void CTestA1GameInterface::onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode) {
    if (sym==SDLK_ESCAPE) {

        CMenuAGameInterface* men = new CMenuAGameInterface(&FSLib.getLibrary());
        men->setEventHandler(SDL_KEYDOWN,&CMenuAGameInterface::onKeyMenu);
        men->setEventHandler(SDL_KEYUP,&CMenuAGameInterface::onKeyMenu);

        men->setPrevious(this);

        FSLib.getLibrary().SendMessage(FSLib.MSGID_RunEngine, (MSGPARM)men);
    } else if (sym==SDLK_SPACE) {
        CFreezeGameInterface* fgi = new CFreezeGameInterface(&FSLib.getLibrary());
        fgi->setEventHandler(SDL_KEYDOWN,&CFreezeGameInterface::onKeyFreeze);
        fgi->setEventHandler(SDL_KEYUP,&CFreezeGameInterface::onKeyFreeze);

        fgi->setPrevious(this);

        FSLib.getLibrary().SendMessage(FSLib.MSGID_RunEngine, (MSGPARM)fgi);
    } else if (sym==SDLK_DELETE) {
        getParent()->SendMessage(FSLib.MSGID_Restart);
    } else if (sym==SDLK_F1) {
        getParent()->SendMessage(FSLib.MSGID_ChangeEngine);
    } else if (sym==SDLK_F2) {
        getParent()->SendMessage(FSLib.MSGID_ReloadEngine,(MSGPARM)this);
    } else if (sym==SDLK_F3) {
        deselect();
        FSDraw.ToggleFullscreen();
        loop();
    }
    CTestAGameInterface::onKeyDown(sym,mod,unicode);
}
