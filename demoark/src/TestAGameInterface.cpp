// TestAGameInterface.cpp: implementation of the CTestAGameInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "TestAGameInterface.h"
#include "FSLibrary.h"
#include "TestA0GameInterface.h"
#include "TestA1GameInterface.h"
#include "TestA2GameInterface.h"
#include "Player.h"

#include "Map.h"

#include "FSWriter.h"

#include <algorithm>

CTestAGameInterface::CTestAGameInterface() {}
CTestAGameInterface::~CTestAGameInterface() {}

int CTestAGameInterface::drawFrame() {

    FSDraw.projectionMode(TRP_PERSPECTIVE,1600);

    for (Uint32 i=0;i<cams.size();i++) {

        cams[i]->locateRenderScene(
            (float)cams[i]->getArea()->x*2,
            (float)cams[i]->getArea()->y*2,
            (float)cams[i]->getArea()->w*2,
            (float)cams[i]->getArea()->h*2
        );

        //cams[i]->scale(2.0,2.0,1.0);

        cams[i]->render();

    }

    Write.render();

    return EXITO;
}

int CTestAGameInterface::onIdle() {

    for (UniverseCollection::iterator it=Cosmos.begin();it!=Cosmos.end();++it) {
        FSUniverse* mapAct = (*it);

        for (ActorCollection::iterator jt=mapAct->actorBegin();jt!=mapAct->actorEnd();++jt) {
            CActorScrollMap* act = (CActorScrollMap*)*jt;
            if (act->eventChange) {
                act->selectActionCandidate();
            }
            act->move();

        }
    }
    
    std::for_each(endTasks.begin(),endTasks.end(),
        [](decltype(endTasks.front())& f) { f(); }
    );

    return EXITO;

}

void CTestAGameInterface::deselect() {
    FSEngine::deselect();

    for (auto it=player.begin();it!=player.end();++it) {
        CPlayer& c =**it;
        c.blockFutureActionCandidates();
    }

}

#include "ActionQuiet0.h"

int CTestAGameInterface::loop() {

    SDL_PumpEvents();

    Uint8* kbarray;
    //grab the keyboard state
    kbarray=SDL_GetKeyState(NULL);

    bool repeated = false;

    for (SDLKey sym = (SDLKey)0; sym< NUM_SDLKEY;sym=(SDLKey)(sym+1)) {
        if (kbarray[sym]) {
            int p = PlayerKeyAlias[sym].player;
            if (p < 255) {
                int j=PlayerKeyAlias[sym].key;
                CAction* newNode = player[p]->getAction();
                if (!newNode) {
                    if (repeated)
                        break;
                    onIdle();
                    sym = (SDLKey) -1;
                    repeated=true;
                    continue;
                }
                 newNode = newNode->getKeydown(j);
                if (dynamic_cast<CActionMove*> (newNode))
                    player[p]->addActionCandidate(newNode);
            }
        }
    }

    return FSEngine::loop();
}

#ifdef MENSAJES_MSGIDS
int CTestAGameInterface::SendMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2) {
    printf("Controlador de Juego :: ");
    return FSMessageHandler::SendMessage(MsgID,Parm1,Parm2);
}
#endif

void CTestAGameInterface::changeMap(CActorScrollMap& actor,int direction) {
    endTasks.push_back([&](){
        CMap* mapaOrigen = (CMap*) actor.getUniverse();

        std::string& nameMapaOrigen=mapaOrigen->getName();

        bool enc=false;
        gate g;
        for (Uint32 i=0;(!enc)&&(i<mapaOrigen->getGates());i++) {
            g=mapaOrigen->getGate(i);
            enc = enc || (((int)g.regionx1<= actor.m_Scrollxy.x) &&
                            ((int)g.regionx2>=actor.m_Scrollxy.x)&&
                            ((int)g.regiony1<=(actor.m_Scrollxy.y+direction))&&
                            ((int)g.regiony2>= (actor.m_Scrollxy.y+direction))&&
                            ((int)actor.m_Scrollxy.getZ()==g.regionz));
        }
        if (enc) {

            actor.m_Scrollxy.set(g.destino_scroll_x,g.destino_scroll_y,g.destino_scroll_z);

            // Si el enlace no es hacia el mismo mapa...
            if (strcmp(nameMapaOrigen.c_str(),g.destino.c_str())!=0) {
                    //Hay una alternativa a este algoritmo en backup(4), la pega de este esque durante un frame tiene un mapa mas en memoria q no sirve de nada.
                    //Adem�s, este no reconvierte un mapa cuando es id�neo, en lugar de crear uno nuevo y destruir el viejo que es m�s costoso.
                    mapaOrigen->decActor((CActorScrollMap*)&actor);

                    CMap* mapaDestino = (CMap*)Cosmos.add(new CMap(g.destino.c_str())); // Si no existe, lo crea, si existe no, y en cualquier caso lo devuelve.

                    if (!mapaDestino->isLoaded())
                        mapaDestino->load();

                    mapaDestino->incActor((FSActor*)&actor);

                    for (Uint32 i=0;i<cams.size();i++) {
                        if (cams[i]->Target()==((FSActor*)&actor)) {
                            cams[i]->resyncUniverse(); // Si el mapa pasa a no ser enfocado por ninguna c�mara, se descarga (unload).
                        } /*else if (strcmp((cams[i]->getUniverse()->getName()).c_str(),nameMapaOrigen.c_str())==0
                        || strcmp((cams[i]->getUniverse()->getName()).c_str(),g.destino.c_str())==0)
                            cams[i]->resyncUniverse();*/
                    }
            }
#ifdef LOG_MAPAS
            printf("\tMapas en memoria: ");
            for (UniverseCollection::iterator it=Cosmos.begin();it!=Cosmos.end();++it) {
                if ((*it)->isLoaded())
                    printf("%s, ",(*it)->getName().c_str());
            }
            printf("%d\n",Cosmos.size());
#endif
        }
    });
}

void CTestAGameInterface::deleteMap(FSUniverse* map) {
    endTasks.push_back([&](){
        Cosmos.erase(map);
    });
}

void CTestAGameInterface::killEnemy(FSActor* victim, FSActor* murder, FSUniverse* map) {
    endTasks.push_back([&](){
        if (map && victim) {
            for (auto it=enemy.begin(),et=enemy.end();it!=et;++it) {
                FSActor* currentEnemy = (FSActor*) *it;
                if (currentEnemy == victim) {
                    enemy.remove(*it);
                    map->decActor(victim);
                    delete victim;
                    victim=NULL;
                    break;
                }
            }
        }
    });
}

void CTestAGameInterface::onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode) {
#ifdef EVENTOS_RAPIDO
    int p=PlayerKeyAlias[sym].player;
    if (p<255) {
        int j=PlayerKeyAlias[sym].key;
        CAction* newNodo=player[p]->getAction()->getKeydown(j);

        if (newNodo!=NULL)
            player[p]->addActionCandidate(newNodo);
    }
#else
    int j,p;
    j=getIndexByKey(sym,&p);

    if (j>=0) {
        CAction* newNodo=player[p]->getAction()->getKeydown(j);
        //if (newNodo->disponible())
        if (newNodo!=NULL)
            player[p]->addActionCandidate(newNodo);
    }
#endif
}

void CTestAGameInterface::onKeyUp(SDLKey sym,SDLMod mod,Uint16 unicode) {
#ifdef EVENTOS_RAPIDO
    int p=PlayerKeyAlias[sym].player;
    if (p<255) {
        int j=PlayerKeyAlias[sym].key;
        CAction* newNodo=player[p]->getAction()->getKeyup(j);
        if (newNodo!=NULL)
            player[p]->removeActionCandidate(newNodo);
    }
#else
    int j,p;
    j=getIndexByKey(sym,&p);

    if (j>=0) {
        CAction* newNodo=player[p]->getAction()->getKeyup(j);
        if (newNodo!=NULL)
            player[p]->removeActionCandidate(newNodo);
    }
#endif
}
#ifndef EVENTOS_RAPIDO
int CTestAGameInterface::getIndexByKey(SDLKey sym,int* p) {
    for (int i=0;i<player.size();i++) {
        int aux=player[i]->KeyAliasFor(sym);
        if (aux>=0) {
            *p=i;
            return aux;
        }
    }
    return -1;
}
#else

void CTestAGameInterface::erasePlayerKeyAlias(int n) {
    PlayerKeyAlias[n].player=255;
}

void CTestAGameInterface::updatePlayerKeyAlias(int n,CPlayer* pyer, int key) {
    Uint8 i=0;
    while ((i<player.size())&&(i<255)) {
        if (pyer==player[i])
            break;
        i++;
    }
    if (i!=255) {
        PlayerKeyAlias[n].player=i;
        PlayerKeyAlias[n].key=key;
    }
}

// Event Handlers

void CTestAGameInterface::onKeyTestA(SDL_Event* event) {
    if (event->type == SDL_KEYDOWN)
        onKeyDown(event->key.keysym.sym,event->key.keysym.mod,event->key.keysym.unicode);
    else
        onKeyUp(event->key.keysym.sym,event->key.keysym.mod,event->key.keysym.unicode);
}

#endif
