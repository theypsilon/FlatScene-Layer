#ifndef __ACTIONCOMBODEMO0_H__
#define __ACTIONCOMBODEMO0_H__

#include "ActionFight.h"

class CActionComboDemo0 : public CActionFight
{
public:
    CActionComboDemo0(CActorScrollMap* player,TiXmlElement* fuente, const char* i);
    ~CActionComboDemo0();
    void execute();
    CAction* mix(ActionCollection mix);

    CAction* clone(CActorScrollMap* exe=NULL);
};

#endif

