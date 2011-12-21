#ifndef __ENEMY_NPC_H__
#define __ENEMY_NPC_H__

#include "Enemy.h"

class CEnemyNPC : public CEnemy {
public:
	CEnemyNPC(CMessageHandler * pmhParent=NULL);
	virtual ~CEnemyNPC();

	virtual void draw(CPoint& offset);
};

#endif