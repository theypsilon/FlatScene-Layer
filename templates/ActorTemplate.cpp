#include "ActorTemplate.h"

CActorTemplate::CActorTemplate(CMessageHandler* pmhParent) : 
// Se ha de llamar a la clase Base para una correcta inicialización
CActor("valor creature",pmhParent) {

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

int CActorTemplate::setUniverse(CUniverse* m) {

	// TODO

	return EXITO;

	// CActor::setUniverse(m) realiza la asignación : inUniverse=m;
}

CSprite* CActorTemplate::getSprite() {

	CSprite* spt;

	// TODO

	return spt;
}

CActor* CActorTemplate::clone() {
	CActor* act;

	// TODO

	return act;
}