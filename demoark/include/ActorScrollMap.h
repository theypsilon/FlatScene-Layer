#ifndef __ACTORSCROLLMAP_H__
#define __ACTORSCROLLMAP_H__

#include "FSActor.h"
#include "Action.h"
#include "GarbageCollector.h"
#include "OrderGraph.h"
#include "RutinaColision.h"

class CTestAGameInterface;
class CActorScrollMap;

typedef std::list<CActorScrollMap*> ActorScrollCollection;


class CActorScrollMap : public FSActor
{
private:
    bool controlled;
public:
    Sint8 dirx,diry;

    CAction* actActual;
    CAction* actUltimo;
    CAction* actNeutro;
    CAction* actUltimoMovimiento;

    CTestAGameInterface& game;

    bool upfloor;
    bool downfloor;
    CRutinaColision* rutinaColision;

    CGarbageCollectorOfActions* garbage;
    ActionCollection listActionCandidate;

    ActorScrollCollection* placeInMA;

    CActorScrollMap(const char* creature,CTestAGameInterface & game);
    virtual ~CActorScrollMap();

    virtual void draw(FSPoint& offset);


    void setAction(CAction* n);
    CAction* getAction();

    bool eventChange;

    void addActionCandidate(CAction* n);
    void removeActionCandidate(CAction* n);
    void selectActionCandidate();

    bool collisionMap(int x, int y);

    virtual void init(std::list<std::string>& activationIds);

    virtual int move();

    virtual void controlThis();
    virtual void noControlThis();
    virtual bool isControlled();

    bool collisionUniverse(int x, int y);

    FSCoordinate m_Scrollxy;

    virtual FSActor* clone();

    static bool orderReferenceActors(CActorScrollMap* x, CActorScrollMap* y);


    static Uint32 MSGID_Damage;	
};

#endif
