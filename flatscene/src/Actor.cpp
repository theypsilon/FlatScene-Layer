#include "Actor.h"
#include "Exception.h"
#include "Universe.h"
#include "SpriteSet.h"

namespace FlatScene {

Actor::Actor(const char* creature) 
: file(0), graph(0), flags(0), inUniverse(nullptr), creature(creature) {}
Actor::~Actor() {}

int Actor::move() {
    return 0;
}

Universe* Actor::getUniverse() {
    return inUniverse;
}
std::string Actor::getCreature() {
    return creature;
}

int Actor::setUniverse(Universe* m) {
    inUniverse=m;

    return 0;
}

int Actor::setSprite(int file,int graph) {
    this->file = file;
    this->graph = graph;

    return 0;
}

const Sprite* Actor::getSprite() {
    throw Exception("FIXME");
    return nullptr;
}

Actor* Actor::clone() {
    throw Exception("CActor no implementa un metodo de clonaci�n por defecto.");
}

} // flatscene
