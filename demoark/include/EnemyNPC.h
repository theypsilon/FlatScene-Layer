#ifndef __ENEMY_NPC_H__
#define __ENEMY_NPC_H__

#include "Enemy.h"

class CEnemyNPC : public CEnemy {
public:
    CEnemyNPC(CTestAGameInterface& game);
    virtual ~CEnemyNPC();

    virtual void draw(FSPoint& offset);
};

#endif