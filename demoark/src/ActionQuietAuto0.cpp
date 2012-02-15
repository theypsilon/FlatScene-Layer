#include "ActionQuietAuto0.h"
#include "Player.h"

CActionQuietAuto0::CActionQuietAuto0(CActorScrollMap* executor,TiXmlElement* fuente,const char* i) : CActionMove(executor,fuente,0,0,i){ 

}

CActionQuietAuto0::~CActionQuietAuto0() { 
    inDelete();
}

void CActionQuietAuto0::inDelete() {
    CActionMove::inDelete();
}

void CActionQuietAuto0::firstExecute() {
    executor->flags = flags[seq];

    lapso = 50 + (rand()%50);
    srand(rand());

    paso=0;

}


void CActionQuietAuto0::execute() {
    if (Chrono.getTick() > timeNew + lapso) {
        int i = rand() % 100;
        srand(rand());
        if (i >= 0 && i < 25) {
            executor->removeActionCandidate(this);
            executor->addActionCandidate((*getBrothers())[1]);
        } else if (i >= 25 && i < 50) {
            executor->removeActionCandidate(this);
            executor->addActionCandidate((*getBrothers())[3]);
        } else if (i >= 50 && i < 75) {
            executor->removeActionCandidate(this);
            executor->addActionCandidate((*getBrothers())[5]);
        } else if (i >= 75 && i < 100) {
            executor->removeActionCandidate(this);
            executor->addActionCandidate((*getBrothers())[7]);
        }
    }

    executor->setSprite(sptset[seq],nextStep());
    
}

void CActionQuietAuto0::lastExecute() {

}

CAction* CActionQuietAuto0::clone(CActorScrollMap* exe) {
    if (!exe)   exe = executor;
    CActionQuietAuto0* ret = new CActionQuietAuto0(exe,NULL,getId());
    cloneThisInto(ret);
    return ret;
}
