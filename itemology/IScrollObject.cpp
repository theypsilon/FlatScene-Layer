#include "IScrollObject.h"

int IScrollObject::instances = 0;

IScrollObject::IScrollObject() 
    : _placeInMA(nullptr), Actor("IScrollObject") {

        instances++;

        place.set(0,0,0);

        // TODO

}
IScrollObject::~IScrollObject() {

    instances--;

    // TODO

}

void IScrollObject::move() {

    // TODO

}

const std::string& IScrollObject::getCreature() {

    // TODO

    return Actor::getCreature();

    // CActor::getCreature() accede al valor que hemos configurado en el Constructor.
}

void IScrollObject::setUniverse(Universe* m) {

    // TODO

    Actor::setUniverse(m);

    // CActor::setUniverse(m) realiza la asignaci�n : inUniverse=m;
}

const Sprite* IScrollObject::getSprite() {

    auto spt = Actor::getSprite();

    // TODO

    return spt;
}

Actor* IScrollObject::clone() {
    Actor* act = nullptr;

    // TODO

    return act;
}
