// TestAGameInterface.cpp: implementation of the CTestAGameInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "TestAGameInterface.h"
#include "FSLibrary.h"
#include "TestA0GameInterface.h"
#include "TestA1GameInterface.h"
#include "TestA2GameInterface.h"

#include "Map.h"

#include "FSControlOutputText.h"

Uint32 CTestAGameInterface::MSGID_ChangeMap=CMessageHandler::getNextMSGID(false);
Uint32 CTestAGameInterface::MSGID_DeleteMap=CMessageHandler::getNextMSGID(false);
Uint32 CTestAGameInterface::MSGID_KillEnemy=CMessageHandler::getNextMSGID(false);

//constructor
CTestAGameInterface::CTestAGameInterface(CMessageHandler* pmhParent) : CEngine(pmhParent)	{
}
//destructor
CTestAGameInterface::~CTestAGameInterface()	{
}

int CTestAGameInterface::drawFrame() {

	CScreen::projectionMode(TRP_PERSPECTIVE,1600);

	for (int i=0;i<cams.size();i++) {

		cams[i]->locateRenderScene(cams[i]->getArea()->X()*2,cams[i]->getArea()->Y()*2,cams[i]->getArea()->W()*2,cams[i]->getArea()->H()*2);

		//cams[i]->scale(2.0,2.0,1.0);

		cams[i]->render();

	}

	Write.render();

	return EXITO;
}

//idle. Main loop.
int CTestAGameInterface::onIdle()	{

	//if (~Chrono.getTick() & 0x01) {

	for (UniverseCollection::iterator it=CMultiverse.begin();it!=CMultiverse.end();++it) {
		CUniverse* mapAct = (*it);

		for (ActorCollection::iterator jt=mapAct->actorBegin();jt!=mapAct->actorEnd();++jt) {
			CActorScrollMap* act = (CActorScrollMap*)*jt;
			if (act->eventChange) {
				act->selectActionCandidate();
			}
			act->move();
			
		}
	}

	//}
	
	readMessages();

	return EXITO;

}

void CTestAGameInterface::deselect() {
	CEngine::deselect();

	for (vector<CPlayer*>::iterator it=player.begin();it!=player.end();++it)
	{
		CPlayer* c =*it;
		c->blockFutureActionCandidates();
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

	return CEngine::loop();
}

#ifdef MENSAJES_MSGIDS
int CTestAGameInterface::SendMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2) {
	printf("Controlador de Juego :: ");
	return CMessageHandler::SendMessage(MsgID,Parm1,Parm2);
}
#endif

void CTestAGameInterface::pendingMessage(Uint32 MsgID, MSGPARM Parm1, MSGPARM Parm2) {
	if (MsgID==MSGID_ChangeMap) {
		CPlayer* p=(CPlayer*)Parm1;
		CMap* mapaOrigen=(CMap*)p->getUniverse();
		string nameMapaOrigen=mapaOrigen->getName();

		int mov_y=(long)Parm2;

		bool enc=false;
		gate g;
		for (int i=0;(!enc)&&(i<mapaOrigen->getGates());i++) {
			g=mapaOrigen->getGate(i);
			enc = enc || ((g.regionx1<= p->m_Scrollxy.getX()) && (g.regionx2>=p->m_Scrollxy.getX())&&(g.regiony1<=(p->m_Scrollxy.getY()+mov_y))&&(g.regiony2>= (p->m_Scrollxy.getY()+mov_y))&&(p->m_Scrollxy.getZ()==g.regionz));
		}
		if (enc) {

			p->m_Scrollxy.set(g.destino_scroll_x,g.destino_scroll_y,g.destino_scroll_z);
			
			// Si el enlace no es hacia el mismo mapa...
			if (strcmp(nameMapaOrigen.c_str(),g.destino.c_str())!=0) {
					//Hay una alternativa a este algoritmo en backup(4), la pega de este esque durante un frame tiene un mapa mas en memoria q no sirve de nada.
					//Además, este no reconvierte un mapa cuando es idóneo, en lugar de crear uno nuevo y destruir el viejo que es más costoso.
					mapaOrigen->decActor((CActorScrollMap*)p);

					CMap* mapaDestino = (CMap*)CMultiverse.add(new CMap(g.destino.c_str()));	// Si no existe, lo crea, si existe no, y en cualquier caso lo devuelve.

					if (!mapaDestino->isLoaded())
						mapaDestino->load();

					mapaDestino->incActor((CActor*)p);

					for (int i=0;i<cams.size();i++) {
						if (cams[i]->Target()==((CActor*)p)) {
							cams[i]->resyncUniverse(); // Si el mapa pasa a no ser enfocado por ninguna cámara, se descarga (unload).
						} /*else if (strcmp((cams[i]->getUniverse()->getName()).c_str(),nameMapaOrigen.c_str())==0 
						|| strcmp((cams[i]->getUniverse()->getName()).c_str(),g.destino.c_str())==0)
							cams[i]->resyncUniverse();*/
					}
			} 
#ifdef LOG_MAPAS
			printf("\tMapas en memoria: ");
			for (UniverseCollection::iterator it=CMultiverse.begin();it!=CMultiverse.end();++it) {
				if ((*it)->isLoaded())
					printf("%s, ",(*it)->getName().c_str());
			}
			printf("%d\n",CMultiverse.size());
#endif
		}

	} else if (MsgID==MSGID_DeleteMap)	{
		CMultiverse.erase((CUniverse*)Parm1);
	} else if (MsgID==MSGID_KillEnemy) {
		void** parm = (void**) Parm2;

		CActor* victim = (CActor*) Parm1;
		CUniverse* map = (CUniverse*) parm[0];
		if (map && victim) {
			CActor* murder = (CActor*) parm[1];
			if (murder) {
				;
			}
			for (list<CEnemy*>::iterator it=enemy.begin(),et=enemy.end();it!=et;++it) {
				if (*it == victim) {
					enemy.remove(*it);
					map->decActor(victim);
					delete victim;
					victim=NULL;
					break;
				}
			}
		}

		delete parm;
	}
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