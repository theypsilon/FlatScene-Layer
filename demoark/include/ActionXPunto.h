#ifndef __ACTION_WALK_AUTO_PUNTO__
#define __ACTION_WALK_AUTO_PUNTO__

#include "Action.h"

class CActionXPunto : public CAction
{
public:
	CActionXPunto(CActorScrollMap* executor,TiXmlElement* fuente,const char* i);
	virtual ~CActionXPunto();
	void inDelete();
	void firstExecute();
	virtual void execute();
	void lastExecute();
};


#endif
