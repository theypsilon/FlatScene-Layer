#ifndef __ACTIONQUIET0_H__
#define __ACTIONQUIET0_H__

#include "ActionMove.h"

class CActionQuiet0 : public CActionMove
{
public:
	CActionQuiet0(CActorScrollMap* player,TiXmlElement* fuente,const char* i);
	virtual ~CActionQuiet0();
	void inDelete();
	void firstExecute();
	virtual void execute();
	void lastExecute();

	CAction* clone(CActorScrollMap* exe=NULL);
};

#endif