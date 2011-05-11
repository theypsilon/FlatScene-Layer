#ifndef __ACTIONMOVE_H__
#define __ACTIONMOVE_H__

#include "Action.h"

class CActionMove : public CAction
{
public:
	Sint8 dirx,diry,movx, movy;
	Uint32 MsgID;
	Uint32 timeNew;
	Uint32 timeOld;

//	CActionMove(int x,int y,int ppf,int i);
	CActionMove(CActorScrollMap* executor,TiXmlElement* fuente,int x,int y,const char* i);
	virtual ~CActionMove();
	virtual void inDelete();
	virtual void firstExecute()=0;
	virtual void execute()=0;
	virtual void lastExecute()=0;
	virtual CAction* mix(ActionCollection mix);
	virtual void use();
	virtual bool elapsed(Uint32 lapso) { return ( timeNew > (timeOld+lapso) ); };

	virtual int getX() { return dirx; };
	virtual int getY() { return diry; };

	static CAction* ActionMoveFactory(CActorScrollMap* executor,TiXmlElement* n);
};

#endif