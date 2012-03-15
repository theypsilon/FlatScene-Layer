#ifndef __UNIVERSE_H__
#define __UNIVERSE_H__

#include <stdio.h>
#include "FSMessageHandler.h"
#include "FSActor.h"
#include <list>
#include <string>

namespace flatscene {

    class Multiverse;

    class Universe {
    private:
        Uint8 numCameras;

    protected:
        bool loaded;
        std::string name;
        Uint8 slot;

        virtual ~Universe();
        Universe(std::string name);

        friend class Multiverse;

        ActorCollection actor; 
    public:
        virtual void load();
        virtual void unload();
        bool isLoaded();
        virtual std::string& getName();
        virtual int changeUniverse(std::string name, Uint8 slot);

        virtual int incActor(Actor* act);
        virtual int decActor(Actor* act);
        virtual ActorCollection::iterator actorBegin();
        virtual ActorCollection::iterator actorEnd();
        virtual void incCameras();
        virtual void decCameras();
        virtual Uint32 numActors();
        virtual Uint8 camaras();

        virtual bool operator == (Universe& uni);
        virtual bool equal (Universe* uni);
    };

} // flatscene

#endif
