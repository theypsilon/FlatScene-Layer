#ifndef __ACTIONWALK0_H__
#define __ACTIONWALK0_H__

#include "ActionMove.h"

class CActionWalk0 : public CActionMove
{
public:
    CActionWalk0(CActorScrollMap* executor,TiXmlElement* fuente,int x,int y,const char* i);
    virtual ~CActionWalk0();
    void inDelete();
    void firstExecute();
    virtual void execute();
    void lastExecute();

    CAction* clone(CActorScrollMap* exe=NULL);
    static bool ThisType(const char* i);
};

#endif