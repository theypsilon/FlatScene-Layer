#ifndef __UNIVERSE_H__
#define __UNIVERSE_H__

#include <stdio.h>
#include "FSMessageHandler.h"
#include "FSActor.h"
#include <list>
#include <string>

class CControlMultiverse;

class CUniverse : public CMessageHandler {
private:
	Uint8 numCameras;

protected:
	bool loaded;
	string name;
	Uint8 slot;

	virtual ~CUniverse();
	CUniverse(string name);

	friend class CControlMultiverse;

	ActorCollection actor; 
public:
	virtual void load();
	virtual void unload();
	bool isLoaded();
	virtual string& getName();
	virtual int changeUniverse(string name, Uint8 slot);

	virtual int incActor(CActor* act);
	virtual int decActor(CActor* act);
	virtual ActorCollection::iterator actorBegin();
	virtual ActorCollection::iterator actorEnd();
	virtual void incCameras();
	virtual void decCameras();
	virtual Uint32 numActors();
	virtual Uint8 camaras();

	virtual bool operator == (CUniverse& uni);
	virtual bool equal (CUniverse* uni);
};

#endif