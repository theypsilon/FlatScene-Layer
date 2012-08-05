#ifndef __ACTOR_H__
#define __ACTOR_H__

#include "Types.h"
#include "Sprite.h"
#include <string>
#include <map>
#include <vector>
#include <deque>

namespace FlatScene {

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

        Actor(const char* creature);
        virtual ~Actor();

        void setSprite(int file,int graph);
        virtual const Sprite* getSprite();

        virtual void move();

        virtual const std::string& getCreature();
        Universe* getUniverse();
        virtual void setUniverse(Universe* m);
        Point renderPoint;

        virtual Actor* clone();

    };

} // flatscene

#endif
