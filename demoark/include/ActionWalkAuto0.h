#ifndef __ACTION_WALK_AUTO_0_H__
#define __ACTION_WALK_AUTO_0_H__

#include "ActionMove.h"

class CActionWalkAuto0 : public CActionMove
{
private:
    int lapso;
public:
    CActionWalkAuto0(CActorScrollMap* executor,TiXmlElement* fuente,int x,int y,const char* i);
    virtual ~CActionWalkAuto0();
    void inDelete();
    void firstExecute();
    virtual void execute();
    void lastExecute();

    CAction* clone(CActorScrollMap* exe=NULL);
    static bool ThisType(const char* i);
};

#endif