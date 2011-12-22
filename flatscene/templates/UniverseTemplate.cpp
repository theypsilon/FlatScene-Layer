#include "UniverseTemplate.h"

CUniverseTemplate::CUniverseTemplate(string name) : CUniverse(name) {

	//TODO

}

CUniverseTemplate::~CUniverseTemplate() {

	if (isLoaded())
		unload();

}


void CUniverseTemplate::load() {

	// TODO

	loaded=true;
}

void CUniverseTemplate::unload() {

	// TODO

	loaded=false;

}

int CUniverseTemplate::changeUniverse(string name,Uint8 slot) {

	// TODO

	return EXITO;

}

int CUniverseTemplate::incActor(FSActor* act) {

	/*		IMPLEMENTACIÓN POR DEFECTO		*/

	/*if (!act) {
		CLibrary::Error("Puntero a CActor nulo");
		return FRACASO;
	}

	CUniverse* u = act->getUniverse();

	if (u) 
		for (ActorCollection::iterator it = u->actorBegin(), jt = u->actorEnd();it!=jt;++it) 
			if (act == *it) {
				CLibrary::Error("Actor actualmente incluido en otro Universe");
				return FRACASO;
			}*/

	// TODO

	return EXITO;
}

int CUniverseTemplate::decActor(FSActor* act) {

	/*		IMPLEMENTACIÓN POR DEFECTO		*/

	/*if (!act) {
		CLibrary::Error("Puntero a CActor nulo");
		return FRACASO;
	}

	for (ActorCollection::iterator it = actorBegin(), jt = actorEnd();it!=jt;++it) 
		if (act == *it) {
			act->setUniverse(NULL);
			actor.remove(act);
			return EXITO;
		}

	CLibrary::Error("Actor actualmente no incluido en este Universe");
	return FRACASO;*/

	// TODO

	return EXITO;
}

void CUniverseTemplate::incCameras() {

	// POR DEFECTO : numCameras++;

	// TODO

}

void CUniverseTemplate::decCameras() {

	/*		IMPLEMENTACIÓN POR DEFECTO		*/

	/*numCameras--;
	if (numCameras==0)
		if (isLoaded())
			unload();*/
}
