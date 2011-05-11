#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "ActorScrollMap.h"

class CEnemy : public CActorScrollMap {
public:
	CEnemy(const char* creature,CMessageHandler* pmhParent=NULL);
	virtual ~CEnemy();
	virtual void init(list<string>& activationIds,int x, int y,int z);

	virtual int onMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2);

	virtual int move();
	virtual CActor* clone();

	static CEnemy* Factory(const char* creature,CMessageHandler* pmhParent=NULL);
};

#endif