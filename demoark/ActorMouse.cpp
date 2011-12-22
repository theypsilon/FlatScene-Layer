#include "ActorMouse.h"
#include "FSControlImages.h"

CActorMouse::CActorMouse(FSMessageHandler * pmhParent) : 
// Se ha de llamar a la clase Base para una correcta inicialización
FSActor("raton",pmhParent) {

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

int CActorMouse::setUniverse(FSUniverse* m) {

	// TODO

	return EXITO;

	// CActor::setUniverse(m) realiza la asignación : inUniverse=m;
}

FSSprite* CActorMouse::getSprite() {

	FSSprite* spt;

	spt = CImg.get(file)->get(graph);

	return spt;
}

FSActor* CActorMouse::clone() {
	FSActor* act = NULL;

	// TODO

	return act;
}