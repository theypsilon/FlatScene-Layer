#include "FSActor.h"
#include "FSLibrary.h"
#include "FSUniverse.h"

FSActor::FSActor(const char* creature) 
: file(0), graph(0), flags(0), inUniverse(NULL), creature(creature) {}
FSActor::~FSActor() {}

int FSActor::move() {
    return EXITO;
}

FSUniverse* FSActor::getUniverse() {
    return inUniverse;
}
std::string FSActor::getCreature() {
    return creature;
}

int FSActor::setUniverse(FSUniverse* m) {
    inUniverse=m;

    return EXITO;
}

int FSActor::setSprite(int file,int graph) {
    this->file = file;
    this->graph = graph;

    return EXITO;
}

FSSprite* FSActor::getSprite() {
    return FSImages::I().get(file)->get(graph);
}

FSActor* FSActor::clone() {
    FSLibrary::I().Error("CActor no implementa un metodo de clonaci�n por defecto.");
    return NULL;
}
