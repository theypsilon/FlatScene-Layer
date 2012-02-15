// TestA4GameInterface.cpp: implementation of the CTestA4GameInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "TestA4GameInterface.h"
#include "FSLibrary.h"
#include "FSWriter.h"

#include "EnemyPunto.h"
#include "EnemyNPC.h"

#include "FreezeGameInterface.h"
#include "MenuAGameInterface.h"

//constructor
CTestA4GameInterface::CTestA4GameInterface(FSMessageHandler * pmhParent) : CTestAGameInterface(pmhParent)
{

}

//destructor
CTestA4GameInterface::~CTestA4GameInterface()
{

        readMessages();
}

//initialization
int  CTestA4GameInterface::onInit() {
    //initialize parent class
    if (FSEngine::onInit() == FRACASO)
        return FRACASO;

    FSDraw.clear();

#ifdef LOG_SISTEMA
    printf("\nTestA0 comienza.\n\n");
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

    activationIds.clear();
    activationIds.push_back("npc_normal_quiet");
    activationIds.push_back("npc_normal_walk");

    TiXmlDocument xmldoc("resources/config.xml");   // Cargamos el archivo de configuraci�n.
    if (!xmldoc.LoadFile()) {    FSLib.Error("resources/config.xml",TE_fileExists); }

    TiXmlHandle input(xmldoc.FirstChildElement("System"));
    int cantNPC=0;

    if (input.FirstChildElement("demonpc").ToElement() && input.FirstChildElement("demonpc").ToElement()->Attribute("cant"))
        cantNPC = atoi(input.FirstChildElement("demonpc").ToElement()->Attribute("cant"));

    if (cantNPC < 0 || cantNPC > 100000)
        cantNPC = 0;

    CMap* mapDemo; 
    if (input.FirstChildElement("demonpc").ToElement() && input.FirstChildElement("demonpc").ToElement()->Attribute("initial-map"))
        mapDemo = (CMap*) Cosmos.add(new CMap(input.FirstChildElement("demonpc").ToElement()->Attribute("initial-map")));
    else
        mapDemo = (CMap*) Cosmos.add(new CMap("mapa2"));
    mapDemo->load();

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
    mapDemo->incActor((FSActor*)player[0]);
    mapDemo->incActor((FSActor*)player[1]);

    cams.push_back(new CScrollCamera((FSActor*)player[0],new FSRectangle(0,0,RESOLUCION_X/2,RESOLUCION_Y),NULL,0.35f));
    cams.push_back(new CScrollCamera((FSActor*)player[0],new FSRectangle(RESOLUCION_X/2,0,RESOLUCION_X/2,RESOLUCION_Y),NULL,0.35f));

    Write.color(Write.inBox("resources/texts0",0),1.0,1.0,1.0,0.7,TCTB_BOX,true);

    Write.line(0,260,10,"ESC para Menu.");


    Write.line(0,460,10,"Teclas:");
    Write.line(0,460,40,"cursores : andar");
    Write.line(0,460,70,"a : atacar");
    Write.line(0,460,100,"F1 : siguiente test");

    Write.line(0,260,450,"Test 4");

    return EXITO;
}

int CTestA4GameInterface::drawFrame() {

    FSDraw.projectionMode(TRP_PERSPECTIVE,1600);

    for (int i=0;i<cams.size();i++) {

        cams[i]->locateRenderScene(cams[i]->getArea()->x*2,cams[i]->getArea()->y*2,cams[i]->getArea()->w*2,cams[i]->getArea()->h*2);

        cams[i]->color((rand()%100)/100.0,(rand()%100)/100.0,(rand()%100)/100.0,(rand()%100)/100.0);

        cams[i]->render();

    }

    Write.render();

    return EXITO;

}


int CTestA4GameInterface::onExit()
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
    printf("\nTestA0 termina.\n\n");
#endif

    return CTestAGameInterface::onExit();
}

void CTestA4GameInterface::onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode) {
    if (sym==SDLK_ESCAPE) {

        CMenuAGameInterface* men = new CMenuAGameInterface(&FSLib.getLibrary());
        men->setEventHandler(SDL_KEYDOWN,&CMenuAGameInterface::onKeyMenu);
        men->setEventHandler(SDL_KEYUP,&CMenuAGameInterface::onKeyMenu);

        men->setPrevious(shared_from_this());

        FSLib.getLibrary().SendMessage(FSLib.MSGID_RunEngine, (MSGPARM)men);
    } else if (sym==SDLK_SPACE) {

        CFreezeGameInterface* fgi = new CFreezeGameInterface(&FSLib.getLibrary());
        fgi->setEventHandler(SDL_KEYDOWN,&CFreezeGameInterface::onKeyFreeze);
        fgi->setEventHandler(SDL_KEYUP,&CFreezeGameInterface::onKeyFreeze);

        fgi->setPrevious(shared_from_this());

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
                for (std::list<CEnemy*>::iterator it=enemy.begin(),et=enemy.end();it!=et;++it) {
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
