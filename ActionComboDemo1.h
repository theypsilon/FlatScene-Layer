#ifndef __ACTIONCOMBODEMO1_H__
#define __ACTIONCOMBODEMO1_H__

#include "ActionFight.h"

class CActionComboDemo1 : public CActionFight
{
public:
	CActionComboDemo1(CActorScrollMap* player,TiXmlElement* fuente,const char* i);
	~CActionComboDemo1();
	void execute();
	CAction* mix(ActionCollection mix);

	CAction* clone(CActorScrollMap* exe=NULL);
};

#endif