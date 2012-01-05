#include "ActionWalk0.h"
#include "Player.h"

CActionWalk0::CActionWalk0(CActorScrollMap* executor,TiXmlElement* fuente,int x, int y,const char* i) : CActionMove(executor,fuente,x,y,i){ 

}

CActionWalk0::~CActionWalk0() { 
	inDelete();
}

void CActionWalk0::inDelete() {
	CActionMove::inDelete();
}

void CActionWalk0::firstExecute() {
	executor->flags = flags[seq];

	executor->dirx = this->dirx;
	executor->diry = this->diry;

	if (executor->actUltimo==executor->actNeutro && executor->actUltimoMovimiento==this && !elapsed(2) && getSucesor()) {
		executor->actActual = getSucesor();	
	}

	paso=0;

	executor->actUltimoMovimiento=this;

}


void CActionWalk0::execute() {
	if (!executor->collisionMap(0,movy)) 
		executor->m_Scrollxy.y+=movy;
	if (!executor->collisionMap(movx,0)) 
		executor->m_Scrollxy.x+=movx;

	executor->setSprite(sptset[seq],nextStep());
	
}

void CActionWalk0::lastExecute() {
	if (movx != 0) {
		if (movx > 0)
			(*getBrothers())[4]->seq=3;
		else
			(*getBrothers())[4]->seq=2;
	} else {
		if (movy < 0)
			(*getBrothers())[4]->seq=1;
		else
			(*getBrothers())[4]->seq=0;
	}
}

bool CActionWalk0::ThisType(const char* i) {
	const char* type[] = { "pj_normal_walk_upleft", "pj_normal_walk_left", "pj_normal_walk_downleft", "pj_normal_walk_up", "pj_normal_walk_down", "pj_normal_walk_upright", "pj_normal_walk_right", "pj_normal_walk_downright" };
	int size = (sizeof(type)/sizeof(type[0]));
	bool ret = false;
	for (int it=0;it<size;it++)
		ret = ret || strcmp(type[it],i)==0;
	return ret;
}

CAction* CActionWalk0::clone(CActorScrollMap* exe) {
	if (!exe)	exe = executor;
	CActionWalk0* ret = new CActionWalk0(exe,NULL,movx,movy,getId());
	cloneThisInto(ret);
	return ret;
}