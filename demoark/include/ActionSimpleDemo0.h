#ifndef __ACTIONSIMPLEDEMO0_H__
#define __ACTIONSIMPLEDEMO0_H__

#include "ActionFight.h"

class CActionSimpleDemo0 : public CActionFight
{
public:
    CActionSimpleDemo0(CActorScrollMap* executor,TiXmlElement* fuente,const char* i);
    ~CActionSimpleDemo0();
    void firstExecute();
    void execute();
    CAction* mix(ActionCollection mix);

    CAction* clone(CActorScrollMap* exe=NULL);
    static bool ThisType(const char* i);
};

#endif