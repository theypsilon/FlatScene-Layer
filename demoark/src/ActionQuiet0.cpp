#include "ActionQuiet0.h"
#include "Player.h"
#include "FSLibrary.h"

CActionQuiet0::CActionQuiet0(CActorScrollMap* executor,TiXmlElement* fuente,const char* i) : CActionMove(executor,fuente,0,0,i){ 

}

CActionQuiet0::~CActionQuiet0() { 
    inDelete();
}

void CActionQuiet0::inDelete() {
    CActionMove::inDelete();
}

void CActionQuiet0::firstExecute() {
    executor->flags = flags[seq];

    paso=0;

}


void CActionQuiet0::execute() {
    if (!executor->collisionMap(0,movy)) 
    	executor->m_Scrollxy.y+=movy;
    if (!executor->collisionMap(movx,0)) 
    	executor->m_Scrollxy.x+=movx;

    executor->setSprite(sptset[seq],nextStep());
    
}

void CActionQuiet0::lastExecute() {

}

CAction* CActionQuiet0::clone(CActorScrollMap* exe) {
    if (!exe)	exe = executor;
    CActionQuiet0* ret = new CActionQuiet0(exe,NULL,getId());
    cloneThisInto(ret);
    return ret;
}