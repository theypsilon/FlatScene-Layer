#include "Universe.h"
#include "Library.h"

namespace FlatScene {

Universe::Universe(std::string name) 
: name(name) , loaded(false), numCameras(0), slot(255) {}

Universe::~Universe() {
    ActorCollection::iterator it ;
    Actor* a ;
    while ( !actor.empty ( ) )
    {
        it = actor.begin ( ) ;
        a = *it ;
        actor.erase ( it ) ;
        delete a ;
    }
}

void Universe::load() {
    loaded=true;
}

void Universe::unload() {
    loaded=false;
}

std::string& Universe::getName() {
    return name;
}

int Universe::incActor(Actor* act) {

    if (!act) {
        throw Exception("Puntero a CActor nulo");
    }

    Universe* u = act->getUniverse();

    if (u) 
        for (ActorCollection::iterator it = u->actorBegin(), jt = u->actorEnd();it!=jt;++it)
            if (act == *it) throw Exception("Actor actualmente incluido en otro Universe");

    return EXIT_SUCCESS;
}

int Universe::decActor(Actor* act) {

    if (!act) throw Exception("Puntero a CActor nulo");

    for (ActorCollection::iterator it = actorBegin(), jt = actorEnd();it!=jt;++it) 
        if (act == *it) {
            act->setUniverse(nullptr);
            actor.erase(it);
            return EXIT_SUCCESS;
        }

    throw Exception("Actor actualmente no incluido en este Universe");
}

ActorCollection::iterator Universe::actorBegin() {
    return actor.begin();
}

ActorCollection::iterator Universe::actorEnd() {
    return actor.end();
}

bool Universe::isLoaded() {
    return loaded;
}

int Universe::changeUniverse(std::string newName,Uint8 slot) {
    if (!isLoaded()) {
        name=newName;
        this->slot = slot;
        return 0;
    }   else
        return -1;
}

void Universe::incCameras() {
    numCameras++;
}

void Universe::decCameras() {
    numCameras--;
    if (numCameras==0)
        if (isLoaded())
            unload();
}

Uint32 Universe::numActors() {
    return actor.size();
}

Uint8 Universe::camaras() {
    return numCameras;
}

bool Universe::operator ==(Universe &uni) {
    return (this->getName()==uni.getName());
}

bool Universe::equal(Universe *uni) {
    return (this->getName()==uni->getName());
}

} // flatscene
