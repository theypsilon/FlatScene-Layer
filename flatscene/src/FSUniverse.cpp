#include "FSUniverse.h"
#include "FSLibrary.h"
#include "FSMultiverseImpl.h"

FSUniverse::FSUniverse(string name) : FSMessageHandler(NULL), name(name) , loaded(false), numCameras(0), slot(255) {

}

FSUniverse::~FSUniverse() {
	if (!FSMultiverse._impl->working) {
		FSLibrary::Error("Solo se puede destruir el Universo a traves del Multiverso.",TE_controlViolation);
	}
	ActorCollection::iterator it ;
	FSActor* a ;
	while ( !actor.empty ( ) )
	{
		it = actor.begin ( ) ;
		a = *it ;
		actor.erase ( it ) ;
		delete a ;
	}
}

void FSUniverse::load() {
	loaded=true;
}

void FSUniverse::unload() {
	loaded=false;
}

string& FSUniverse::getName() {
	return name;
}

int FSUniverse::incActor(FSActor* act) {

	if (!act) {
		FSLibrary::Error("Puntero a CActor nulo");
		return FRACASO;
	}

	FSUniverse* u = act->getUniverse();

	if (u) 
		for (ActorCollection::iterator it = u->actorBegin(), jt = u->actorEnd();it!=jt;++it) 
			if (act == *it) {
				FSLibrary::Error("Actor actualmente incluido en otro Universe");
				return FRACASO;
			}

	return EXITO;
}

int FSUniverse::decActor(FSActor* act) {

	if (!act) {
		FSLibrary::Error("Puntero a CActor nulo");
		return FRACASO;
	}

	for (ActorCollection::iterator it = actorBegin(), jt = actorEnd();it!=jt;++it) 
		if (act == *it) {
			act->setUniverse(NULL);
			actor.erase(it);
			return EXITO;
		}

	FSLibrary::Error("Actor actualmente no incluido en este Universe");
	return FRACASO;
}

ActorCollection::iterator FSUniverse::actorBegin() {
	return actor.begin();
}

ActorCollection::iterator FSUniverse::actorEnd() {
	return actor.end();
}

bool FSUniverse::isLoaded() {
	if (!FSMultiverse._impl->working && slot == 255) {
		FSLibrary::Error("No se ha inicializado este mapa, a traves del Multiverso. Inutilizable.");
		return false;
	}
	return loaded;
}

int FSUniverse::changeUniverse(string newName,Uint8 slot) {
	if (!isLoaded()) {
		name=newName;
		this->slot = slot;
		return 0;
	}	else
		return -1;
}

void FSUniverse::incCameras() {
	numCameras++;
}

void FSUniverse::decCameras() {
	numCameras--;
	if (numCameras==0)
		if (isLoaded())
			unload();
}

Uint32 FSUniverse::numActors() {
	return actor.size();
}

Uint8 FSUniverse::camaras() {
	return numCameras;
}

bool FSUniverse::operator ==(FSUniverse &uni) {
	return (this->getName()==uni.getName());
}

bool FSUniverse::equal(FSUniverse *uni) {
	return (this->getName()==uni->getName());
}
