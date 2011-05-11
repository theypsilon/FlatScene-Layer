#include "Universe.h"
#include "Library.h"
#include "ControlMultiverse.h"

CUniverse::CUniverse(string name) : CMessageHandler(NULL), name(name) , loaded(false), numCameras(0), slot(255) {

}

CUniverse::~CUniverse() {
	if (!CMultiverse.working) {
		CLibrary::Error("Solo se puede destruir el Universo a traves del Multiverso.",TE_controlViolation);
	}
	ActorCollection::iterator it ;
	CActor* a ;
	while ( !actor.empty ( ) )
	{
		it = actor.begin ( ) ;
		a = *it ;
		actor.erase ( it ) ;
		delete a ;
	}
}

void CUniverse::load() {
	loaded=true;
}

void CUniverse::unload() {
	loaded=false;
}

string& CUniverse::getName() {
	return name;
}

int CUniverse::incActor(CActor* act) {

	if (!act) {
		CLibrary::Error("Puntero a CActor nulo");
		return FRACASO;
	}

	CUniverse* u = act->getUniverse();

	if (u) 
		for (ActorCollection::iterator it = u->actorBegin(), jt = u->actorEnd();it!=jt;++it) 
			if (act == *it) {
				CLibrary::Error("Actor actualmente incluido en otro Universe");
				return FRACASO;
			}

	return EXITO;
}

int CUniverse::decActor(CActor* act) {

	if (!act) {
		CLibrary::Error("Puntero a CActor nulo");
		return FRACASO;
	}

	for (ActorCollection::iterator it = actorBegin(), jt = actorEnd();it!=jt;++it) 
		if (act == *it) {
			act->setUniverse(NULL);
			actor.erase(it);
			return EXITO;
		}

	CLibrary::Error("Actor actualmente no incluido en este Universe");
	return FRACASO;
}

ActorCollection::iterator CUniverse::actorBegin() {
	return actor.begin();
}

ActorCollection::iterator CUniverse::actorEnd() {
	return actor.end();
}

bool CUniverse::isLoaded() {
	if (!CMultiverse.working && slot == 255) {
		CLibrary::Error("No se ha inicializado este mapa, a traves del Multiverso. Inutilizable.");
		return false;
	}
	return loaded;
}

int CUniverse::changeUniverse(string newName,Uint8 slot) {
	if (!isLoaded()) {
		name=newName;
		this->slot = slot;
		return 0;
	}	else
		return -1;
}

void CUniverse::incCameras() {
	numCameras++;
}

void CUniverse::decCameras() {
	numCameras--;
	if (numCameras==0)
		if (isLoaded())
			unload();
}

Uint32 CUniverse::numActors() {
	return actor.size();
}

Uint8 CUniverse::camaras() {
	return numCameras;
}

bool CUniverse::operator ==(CUniverse &uni) {
	return (this->getName()==uni.getName());
}

bool CUniverse::equal(CUniverse *uni) {
	return (this->getName()==uni->getName());
}