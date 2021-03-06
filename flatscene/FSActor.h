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

class CActor;
class CRutinaColision;
class CUniverse;

typedef std::list<CActor*> ActorCollection;

class CActor : public CMessageHandler
{
private:
	string creature;
	CUniverse* inUniverse;
public:

	int file;
	int graph;
	Uint8 flags;

	CActor(const char* creature,CMessageHandler* pmhParent=NULL);
	virtual ~CActor();

	int setSprite(int file,int graph);
	virtual CSprite* getSprite();

	virtual int move();

	virtual string getCreature();
	CUniverse* getUniverse();
	virtual int setUniverse(CUniverse* m);
	CPoint renderPoint;

	virtual CActor* clone();

};

#endif
