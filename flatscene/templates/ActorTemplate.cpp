#include "ActorTemplate.h"

CActorTemplate::CActorTemplate(MessageHandler* pmhParent) : 
// Se ha de llamar a la clase Base para una correcta inicializaci�n
Actor("valor creature",pmhParent) {

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

int CActorTemplate::setUniverse(Universe* m) {

	// TODO

	return EXITO;

	// CActor::setUniverse(m) realiza la asignaci�n : inUniverse=m;
}

Sprite* CActorTemplate::getSprite() {

	Sprite* spt;

	// TODO

	return spt;
}

Actor* CActorTemplate::clone() {
	Actor* act;

	// TODO

	return act;
}