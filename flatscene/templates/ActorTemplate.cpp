#include "ActorTemplate.h"

CActorTemplate::CActorTemplate(FSMessageHandler* pmhParent) : 
// Se ha de llamar a la clase Base para una correcta inicializaci�n
FSActor("valor creature",pmhParent) {

	// TODO
	
}
CActorTemplate::~CActorTemplate() {

	// TODO

}

int CActorTemplate::move() {

	// TODO

	return EXITO;
}

string CActorTemplate::getCreature() {

	// TODO

	return string("cadena personalizable");

	// CActor::getCreature() accede al valor que hemos configurado en el Constructor.
}

int CActorTemplate::setUniverse(FSUniverse* m) {

	// TODO

	return EXITO;

	// CActor::setUniverse(m) realiza la asignaci�n : inUniverse=m;
}

FSSprite* CActorTemplate::getSprite() {

	FSSprite* spt;

	// TODO

	return spt;
}

FSActor* CActorTemplate::clone() {
	FSActor* act;

	// TODO

	return act;
}