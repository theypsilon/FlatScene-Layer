#ifndef __ACTIONFIGHT_H__
#define __ACTIONFIGHT_H__

#include "Action.h"

class CActionFight : public CAction
{
public:

    Uint32 time1;
    Uint32 MsgID;
    int idProvisional;

    CActionFight(CActorScrollMap* executor, TiXmlElement* fuente,const char* i);
    virtual ~CActionFight();
    virtual void inDelete();
    virtual void use() { time1=Chrono.getTick(); CAction::use(); };
    	//void recycle() {
    virtual void execute()=0;

    virtual bool elapsed(Uint32 lapso) { return ( Chrono.getTick() > (time1+lapso) ); };

    static CAction* ActionFightFactory(CActorScrollMap* executor,TiXmlElement* n);
};

#endif

