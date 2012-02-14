#ifndef __ENEMY_PUNTO_H__
#define __ENEMY_PUNTO_H__

#include "Enemy.h"
#include "ActionXPunto.h"

class CEnemyPunto : public CEnemy {
public:
    CEnemyPunto(FSMessageHandler * pmhParent=NULL);
    virtual ~CEnemyPunto();

    void init(list<string>& activationIds,int x, int y,int z);

    FSActor* clone();
};

#endif