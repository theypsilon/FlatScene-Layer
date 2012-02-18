#include "ActionSimpleDemo0.h"
#include "Player.h"
#include "Enemy.h"
#include "Map.h"
#include "TestAGameInterface.h"

CActionSimpleDemo0::CActionSimpleDemo0 (CActorScrollMap* executor,TiXmlElement* fuente,const char* i) : CActionFight(executor,fuente,i) { 
}

CActionSimpleDemo0::~CActionSimpleDemo0() { 
    inDelete();
}

void CActionSimpleDemo0::firstExecute() {
    
    
    

    if ( executor->dirx > 0 ) {
        seq=0;
        executor->flags = 0;
    }   else if (  executor->dirx < 0 ) {
        seq=0;
        executor->flags = 1;
    }   else if (  executor->diry < 0 ) {
        seq = 1;
        executor->flags = 0;
    }   else {
        seq=2;
        executor->flags = 0;
    }

    
}

void CActionSimpleDemo0::execute() {
    executor->setSprite(sptset[seq],nextStep());

    int x = executor->m_Scrollxy.getX();
    int y = executor->m_Scrollxy.getY();
    int z = executor->m_Scrollxy.getZ();

    int flagsE = executor->flags;

    std::stack<CActorScrollMap*> presas;

    RectArea* rA = executor->getSprite()->getArea(2);
    CMap* map = (CMap*)executor->getUniverse();

    Uint32 w = map->getW();
    Uint32 h = map->getH();

    if (rA) {
        RectArea::iterator beginMobil = rA->begin();
        RectArea::iterator finMobil = rA->end();

        FSRectangle r1;
        FSRectangle r2;

        for (RectArea::iterator itR1 = beginMobil; itR1 != finMobil; ++itR1) {
            r1 = **itR1;
            r1.y+=y;
            r1.h+=y;

            if (flagsE & 0x01) {
                Sint16 aux= - r1.x+x;
                r1.x = - r1.w +x;
                r1.w = aux;
            } else {
                r1.x+=x;
                r1.w+=x;
            }

            int Mx = r1.x / map->getTileW() - map->getPrecissionPlus();
            int initMy = r1.y/ map->getTileH() - map->getPrecissionPlus();

            int maxMx = r1.w/map->getTileW() + map->getPrecissionPlus();
            int maxMy = r1.h/map->getTileH() + map->getPrecissionPlus();

            for (;Mx <= maxMx;Mx++) {
                if (Mx < 0 || Mx >= w)
                    continue;

                int My = initMy;
                for (;My <= maxMy;My++) {
                    if (My < 0 || My >= h)
                        continue;

                    for (ActorScrollCollection::iterator itA = map->MA[z][Mx][My]->begin(),finA = map->MA[z][Mx][My]->end();itA!=finA;++itA) {
                        CActorScrollMap* obs = *itA;

                        if (obs==executor)
                            continue;

                        for (RectArea::iterator itR2 = obs->getSprite()->getArea(1)->begin(),finObs = obs->getSprite()->getArea(1)->end(); itR2!=finObs;++itR2) {
                            r2 = **itR2;
                            r2.x+=obs->m_Scrollxy.x;
                            r2.w+=obs->m_Scrollxy.x;
                            r2.y+=obs->m_Scrollxy.y;
                            r2.h+=obs->m_Scrollxy.y;
                            if (r2.intersect(r1) && (typeid(*obs)!=typeid(*executor)))
                                presas.push(obs);
                        }
                    }
                }
            }
        }

    }

    while (!presas.empty()) {
        CEnemy* presa = dynamic_cast<CEnemy*>(presas.top());
        presas.pop();
        if (presa)
            presa->damage(executor);
    }

    if (paso == 0) {
        executor->removeActionCandidate(this);
        executor->setAction(executor->actNeutro);
    }
}

CAction* CActionSimpleDemo0::mix(ActionCollection mix) {
    return (getBrothers()!=NULL?(*getBrothers())[0]:NULL);
}

bool CActionSimpleDemo0::ThisType(const char* i) {
    const char* type[] = { "pj_normal_ba", "pj_normal_sa", "pj_normal_pba" };
    int size = (sizeof(type)/sizeof(type[0]));
    bool ret = false;
    for (int it=0;it<size;it++)
        ret = ret || strcmp(type[it],i)==0;
    return ret;
}

CAction* CActionSimpleDemo0::clone(CActorScrollMap* exe) {
    if (!exe)   exe = executor;
    CActionSimpleDemo0* ret = new CActionSimpleDemo0(exe,NULL,getId());
    cloneThisInto(ret);
    return ret;
}
