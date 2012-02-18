#include "IScrollObject.h"

int IScrollObject::instances = 0;

IScrollObject::IScrollObject(CMessageHandler* pmhParent) : 
// Se ha de llamar a la clase Base para una correcta inicialización
placeInMA(NULL), CActor("IScrollObject",pmhParent) {

	instances++;

	place.set(0,0,0);

	// TODO
	
}
IScrollObject::~IScrollObject() {

	instances--;

	// TODO

}

int IScrollObject::move() {

	// TODO

	return EXITO;
}

string IScrollObject::getCreature() {

	// TODO

	return CActor::getCreature();

	// CActor::getCreature() accede al valor que hemos configurado en el Constructor.
}

int IScrollObject::setUniverse(CUniverse* m) {

	// TODO

	return CActor::setUniverse(m);

	// CActor::setUniverse(m) realiza la asignación : inUniverse=m;
}

CSprite* IScrollObject::getSprite() {

	CSprite* spt = CActor::getSprite();

	// TODO

	return spt;
}

CActor* IScrollObject::clone() {
	CActor* act;

	// TODO

	return act;
}