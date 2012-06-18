#include "FSActor.h"
#include "FSLibrary.h"
#include "FSUniverse.h"

namespace FlatScene {

Actor::Actor(const char* creature) 
: file(0), graph(0), flags(0), inUniverse(NULL), creature(creature) {}
Actor::~Actor() {}

int Actor::move() {
    return EXITO;
}

Universe* Actor::getUniverse() {
    return inUniverse;
}
std::string Actor::getCreature() {
    return creature;
}

int Actor::setUniverse(Universe* m) {
    inUniverse=m;

    return EXITO;
}

int Actor::setSprite(int file,int graph) {
    this->file = file;
    this->graph = graph;

    return EXITO;
}

const Sprite* Actor::getSprite() {
    return Images::I().get(file)->get(graph);
}

Actor* Actor::clone() {
    Library::I().Error("CActor no implementa un metodo de clonaci�n por defecto.");
    return NULL;
}

} // flatscene
