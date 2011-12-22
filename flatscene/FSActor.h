#ifndef __ACTOR_H__
#define __ACTOR_H__

#include "FSMessageHandler.h"
#include "FSCoordinate.h"
#include "FSSprite.h"
#include <string>
#include <map>
#include <vector>
#include <deque>
using namespace std;

class FSActor;
class CRutinaColision;
class FSUniverse;

typedef std::list<FSActor*> ActorCollection;

class FSActor : public FSMessageHandler
{
private:
	string creature;
	FSUniverse* inUniverse;
public:

	int file;
	int graph;
	Uint8 flags;

	FSActor(const char* creature,FSMessageHandler * pmhParent=NULL);
	virtual ~FSActor();

	int setSprite(int file,int graph);
	virtual FSSprite* getSprite();

	virtual int move();

	virtual string getCreature();
	FSUniverse* getUniverse();
	virtual int setUniverse(FSUniverse* m);
	FSPoint renderPoint;

	virtual FSActor* clone();

};

#endif
