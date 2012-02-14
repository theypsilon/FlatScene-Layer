#ifndef __UNIVERSE_H__
#define __UNIVERSE_H__

#include <stdio.h>
#include "FSMessageHandler.h"
#include "FSActor.h"
#include <list>
#include <string>

class FSMultiverse;

class FSUniverse : public FSMessageHandler {
private:
    Uint8 numCameras;

protected:
    bool loaded;
    string name;
    Uint8 slot;

    virtual ~FSUniverse();
    FSUniverse(string name);

    friend class FSMultiverse;

    ActorCollection actor; 
public:
    virtual void load();
    virtual void unload();
    bool isLoaded();
    virtual std::string& getName();
    virtual int changeUniverse(std::string name, Uint8 slot);

    virtual int incActor(FSActor* act);
    virtual int decActor(FSActor* act);
    virtual ActorCollection::iterator actorBegin();
    virtual ActorCollection::iterator actorEnd();
    virtual void incCameras();
    virtual void decCameras();
    virtual Uint32 numActors();
    virtual Uint8 camaras();

    virtual bool operator == (FSUniverse& uni);
    virtual bool equal (FSUniverse* uni);
};

#endif
