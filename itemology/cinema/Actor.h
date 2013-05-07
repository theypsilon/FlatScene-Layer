#ifndef CINEMA_ACTOR__
#define CINEMA_ACTOR__

#include "Types.h"
#include "Sprite.h"
#include <string>
#include <map>
#include <vector>
#include <deque>

namespace Cinema {

    class Actor;
    class CRutinaColision;
    class Universe;

    typedef std::list<Actor*> ActorCollection;

    class Actor {
    private:
        std::string creature;
        Universe* inUniverse;
    public:

        int file;
        int graph;
        unsigned char flags;

        Actor(std::string creature);
        virtual ~Actor();

        void setSprite(int file,int graph);
        virtual const FlatScene::Sprite& getSprite();

        virtual void move();

        virtual const std::string& getCreature();
        Universe& getUniverse();
        virtual void setUniverse(Universe* m);
        FlatScene::Point renderPoint;

        virtual Actor* clone();

    };

}

#include "Actor-impl.h"

#endif