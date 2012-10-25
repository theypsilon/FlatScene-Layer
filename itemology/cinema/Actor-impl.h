#ifndef CINEMA_ACTOR_IMPL__
#define CINEMA_ACTOR_IMPL__

#include "Actor.h"
#include "Exception.h"
#include "Universe.h"
#include "ImageSet.h"

namespace Cinema {
    inline Actor::Actor(const char* creature) 
        : file(0), graph(0), flags(0), inUniverse(nullptr), creature(creature) 
    {}

    inline Actor::~Actor() {}

    inline void Actor::move() {}

    inline Universe* Actor::getUniverse() {
        return inUniverse;
    }

    inline const std::string& Actor::getCreature() {
        return creature;
    }

    inline void Actor::setUniverse(Universe* m) {
        inUniverse=m;
    }

    inline void Actor::setSprite(int file,int graph) {
        this->file = file;
        this->graph = graph;
    }

    inline const FlatScene::Sprite* Actor::getSprite() {
        throw FlatScene::Exception("FIXME");
        return nullptr;
    }

    inline Actor* Actor::clone() {
        throw FlatScene::Exception("CActor no implementa un metodo de clonaci�n por defecto.");
    }
} // Cinema

#endif //CINEMA_ACTOR_IMPL__