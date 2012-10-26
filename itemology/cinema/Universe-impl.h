#ifndef CINEMA_UNIVERSE_IMPL__
#define CINEMA_UNIVERSE_IMPL__

#include <stdio.h>
#include "Actor.h"
#include <list>
#include <string>

namespace Cinema {

    inline Universe::Universe(std::string name) 
    : name(name) , loaded(false), numCameras(0), slot(255) {}

    inline Universe::~Universe() {
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

    inline void Universe::load() {
        loaded=true;
    }

    inline void Universe::unload() {
        loaded=false;
    }

    inline std::string& Universe::getName() {
        return name;
    }

    inline int Universe::incActor(Actor* act) {

        if (!act) {
            throw FlatScene::Exception("Puntero a CActor nulo");
        }

        Universe* u = act->getUniverse();

        if (u) 
            for (ActorCollection::iterator it = u->actorBegin(), jt = u->actorEnd();it!=jt;++it)
                if (act == *it) throw FlatScene::Exception("Actor actualmente incluido en otro Universe");

        return EXIT_SUCCESS;
    }

    inline int Universe::decActor(Actor* act) {

        if (!act) throw FlatScene::Exception("Puntero a CActor nulo");

        for (ActorCollection::iterator it = actorBegin(), jt = actorEnd();it!=jt;++it) 
            if (act == *it) {
                act->setUniverse(nullptr);
                actor.erase(it);
                return EXIT_SUCCESS;
            }

        throw FlatScene::Exception("Actor actualmente no incluido en este Universe");
    }

    inline ActorCollection::iterator Universe::actorBegin() {
        return actor.begin();
    }

    inline ActorCollection::iterator Universe::actorEnd() {
        return actor.end();
    }

    inline bool Universe::isLoaded() {
        return loaded;
    }

    inline int Universe::changeUniverse(std::string newName,unsigned char slot) {
        if (!isLoaded()) {
            name=newName;
            this->slot = slot;
            return 0;
        }   else
            return -1;
    }

    inline void Universe::incCameras() {
        numCameras++;
    }

    inline void Universe::decCameras() {
        numCameras--;
        if (numCameras==0)
            if (isLoaded())
                unload();
    }

    inline unsigned int Universe::numActors() {
        return actor.size();
    }

    inline unsigned char Universe::camaras() {
        return numCameras;
    }

    inline bool Universe::operator ==(Universe &uni) {
        return (this->getName()==uni.getName());
    }

    inline bool Universe::equal(Universe *uni) {
        return (this->getName()==uni->getName());
    }

} // Cinema

#endif //CINEMA_UNIVERSE_IMPL__