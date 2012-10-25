#ifndef CINEMA_UNIVERSE__
#define CINEMA_UNIVERSE__

#include <stdio.h>
#include "Actor.h"
#include <list>
#include <string>

namespace Cinema {
    class Multiverse;

    class Universe {
    private:
        unsigned char numCameras;

    protected:
        bool loaded;
        std::string name;
        unsigned char slot;

        virtual ~Universe();
        Universe(std::string name);

        friend class Multiverse;

        ActorCollection actor; 
    public:
        virtual void load();
        virtual void unload();
        bool isLoaded();
        virtual std::string& getName();
        virtual int changeUniverse(std::string name, unsigned char slot);

        virtual int incActor(Actor* act);
        virtual int decActor(Actor* act);
        virtual ActorCollection::iterator actorBegin();
        virtual ActorCollection::iterator actorEnd();
        virtual void incCameras();
        virtual void decCameras();
        virtual unsigned int numActors();
        virtual unsigned char camaras();

        virtual bool operator == (Universe& uni);
        virtual bool equal (Universe* uni);
    };
} // Cinema

#include "Universe-impl.h"

#endif // CINEMA_UNIVERSE__