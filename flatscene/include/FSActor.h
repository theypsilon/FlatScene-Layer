#ifndef __ACTOR_H__
#define __ACTOR_H__

#include "FSMessageHandler.h"
#include "FSTypes.h"
#include "FSSprite.h"
#include <string>
#include <map>
#include <vector>
#include <deque>

namespace flatscene {

    class FSActor;
    class CRutinaColision;
    class FSUniverse;

    typedef std::list<FSActor*> ActorCollection;

    class FSActor {
    private:
        std::string creature;
        FSUniverse* inUniverse;
    public:

        int file;
        int graph;
        Uint8 flags;

        FSActor(const char* creature);
        virtual ~FSActor();

        int setSprite(int file,int graph);
        virtual const FSSprite* getSprite();

        virtual int move();

        virtual std::string getCreature();
        FSUniverse* getUniverse();
        virtual int setUniverse(FSUniverse* m);
        FSPoint renderPoint;

        virtual FSActor* clone();

    };

} // flatscene

#endif
