#include "FSUniverse.h"
#include "FSLibrary.h"
#include "FSMultiverseImpl.h"

FSUniverse::FSUniverse(std::string name) 
: name(name) , loaded(false), numCameras(0), slot(255) {}

FSUniverse::~FSUniverse() {
    ActorCollection::iterator it ;
    FSActor* a ;
    while ( !actor.empty ( ) )
    {
        it = actor.begin ( ) ;
        a = *it ;
        actor.erase ( it ) ;
        delete a ;
    }
}

void FSUniverse::load() {
    loaded=true;
}

void FSUniverse::unload() {
    loaded=false;
}

std::string& FSUniverse::getName() {
    return name;
}

int FSUniverse::incActor(FSActor* act) {

    if (!act) {
        FSLibrary::I().Error("Puntero a CActor nulo");
        return FRACASO;
    }

    FSUniverse* u = act->getUniverse();

    if (u) 
        for (ActorCollection::iterator it = u->actorBegin(), jt = u->actorEnd();it!=jt;++it)
            if (act == *it) {
                FSLibrary::I().Error("Actor actualmente incluido en otro Universe");
                return FRACASO;
            }

    return EXITO;
}

int FSUniverse::decActor(FSActor* act) {

    if (!act) {
        FSLibrary::I().Error("Puntero a CActor nulo");
        return FRACASO;
    }

    for (ActorCollection::iterator it = actorBegin(), jt = actorEnd();it!=jt;++it) 
        if (act == *it) {
            act->setUniverse(NULL);
            actor.erase(it);
            return EXITO;
        }

    FSLibrary::I().Error("Actor actualmente no incluido en este Universe");
    return FRACASO;
}

ActorCollection::iterator FSUniverse::actorBegin() {
    return actor.begin();
}

ActorCollection::iterator FSUniverse::actorEnd() {
    return actor.end();
}

bool FSUniverse::isLoaded() {
    return loaded;
}

int FSUniverse::changeUniverse(std::string newName,Uint8 slot) {
    if (!isLoaded()) {
        name=newName;
        this->slot = slot;
        return 0;
    }   else
        return -1;
}

void FSUniverse::incCameras() {
    numCameras++;
}

void FSUniverse::decCameras() {
    numCameras--;
    if (numCameras==0)
        if (isLoaded())
            unload();
}

Uint32 FSUniverse::numActors() {
    return actor.size();
}

Uint8 FSUniverse::camaras() {
    return numCameras;
}

bool FSUniverse::operator ==(FSUniverse &uni) {
    return (this->getName()==uni.getName());
}

bool FSUniverse::equal(FSUniverse *uni) {
    return (this->getName()==uni->getName());
}
