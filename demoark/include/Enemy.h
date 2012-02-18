#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "ActorScrollMap.h"

class CEnemy : public CActorScrollMap {
public:
    CEnemy(const char* creature,CTestAGameInterface& game);
    virtual ~CEnemy();
    virtual void init(std::list<std::string>& activationIds,int x, int y,int z);

    virtual void damage(CActorScrollMap* executor);

    virtual int move();
    virtual FSActor* clone();

    static CEnemy* Factory(const char* creature,CTestAGameInterface& game);
};

#endif