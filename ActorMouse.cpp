#include "ActorMouse.h"

CActorMouse::CActorMouse(CMessageHandler* pmhParent) : 
// Se ha de llamar a la clase Base para una correcta inicialización
CActor("raton",pmhParent) {

	// TODO
	
}
CActorMouse::~CActorMouse() {

	// TODO

}

int CActorMouse::move() {

	// TODO

	return EXITO;
}

string CActorMouse::getCreature() {

	// TODO

	return string("cadena personalizable");

	// CActor::getCreature() accede al valor que hemos configurado en el Constructor.
}

int CActorMouse::setUniverse(CUniverse* m) {

	// TODO

	return EXITO;

	// CActor::setUniverse(m) realiza la asignación : inUniverse=m;
}

CSprite* CActorMouse::getSprite() {

	CSprite* spt;

	spt = CImg.get(file)->get(graph);

	return spt;
}

CActor* CActorMouse::clone() {
	CActor* act;

	// TODO

	return act;
}