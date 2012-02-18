#include "IScrollObject.h"

int IScrollObject::instances = 0;

IScrollObject::IScrollObject() 
: placeInMA(NULL), FSActor("IScrollObject") {

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

std::string IScrollObject::getCreature() {

	// TODO

	return FSActor::getCreature();

	// CActor::getCreature() accede al valor que hemos configurado en el Constructor.
}

int IScrollObject::setUniverse(FSUniverse* m) {

	// TODO

	return FSActor::setUniverse(m);

	// CActor::setUniverse(m) realiza la asignación : inUniverse=m;
}

FSSprite* IScrollObject::getSprite() {

	FSSprite* spt = FSActor::getSprite();

	// TODO

	return spt;
}

FSActor* IScrollObject::clone() {
	FSActor* act;

	// TODO

	return act;
}