#ifndef __ACTOR_H__
#define __ACTOR_H__

#include "FSMessageHandler.h"
#include "FSTypes.h"
#include "FSSprite.h"
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
        Uint8 flags;

        Actor(const char* creature);
        virtual ~Actor();

        int setSprite(int file,int graph);
        virtual const Sprite* getSprite();

        virtual int move();

        virtual std::string getCreature();
        Universe* getUniverse();
        virtual int setUniverse(Universe* m);
        Point renderPoint;

        virtual Actor* clone();

    };

} // flatscene

#endif
