#ifndef __ACTORSCROLLMAP_H__
#define __ACTORSCROLLMAP_H__

#include "FSActor.h"
#include "Action.h"
#include "GarbageCollector.h"
#include "OrderGraph.h"

using namespace std;


class CRutinaColision;
class CActorScrollMap;

typedef list<CActorScrollMap*> ActorScrollCollection;


class CActorScrollMap : public CActor
{
private:
	bool controlled;
public:
	Sint8 dirx,diry;

	CAction* actActual;
	CAction* actUltimo;
	CAction* actNeutro;
	CAction* actUltimoMovimiento;

	bool upfloor;
	bool downfloor;
	CRutinaColision* rutinaColision;

	CGarbageCollectorOfActions* garbage;
	ActionCollection listActionCandidate;

	ActorScrollCollection* placeInMA;

	CActorScrollMap(const char* creature,CMessageHandler* pmhParent=NULL);
	virtual ~CActorScrollMap();

	virtual void draw(CPoint& offset);


	void setAction(CAction* n);
	CAction* getAction();

	bool eventChange;

	void addActionCandidate(CAction* n);
	void removeActionCandidate(CAction* n);
	void selectActionCandidate();

	bool collisionMap(int x, int y);

	virtual void init(list<string>& activationIds);

	virtual int move();

	virtual void controlThis();
	virtual void noControlThis();
	virtual bool isControlled();

	bool collisionUniverse(int x, int y);

	CCoordinate m_Scrollxy;

	virtual CActor* clone();

	static bool orderReferenceActors(CActorScrollMap* x, CActorScrollMap* y);


	static Uint32 MSGID_Damage;	
};

#endif
