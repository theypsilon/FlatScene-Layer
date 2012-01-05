#include "ActionWalkAuto0.h"
#include "Player.h"

CActionWalkAuto0::CActionWalkAuto0(CActorScrollMap* executor,TiXmlElement* fuente,int x, int y,const char* i) : CActionMove(executor,fuente,x,y,i){ 

}

CActionWalkAuto0::~CActionWalkAuto0() { 
	inDelete();
}

void CActionWalkAuto0::inDelete() {
	CActionMove::inDelete();
}

void CActionWalkAuto0::firstExecute() {
	executor->flags = flags[seq];

	/*if (executor->actUltimo==executor->actNeutro && executor->actUltimoMovimiento==this && !elapsed(2) && getSucesor()) {
		executor->actActual = getSucesor();	
	}*/

	paso=0;

	lapso = rand()%60;
	srand(rand());

	executor->actUltimoMovimiento=this;
}


void CActionWalkAuto0::execute() {
	if (Chrono.getTick() > (timeNew + 60-lapso)) {
		executor->removeActionCandidate(this);
		executor->addActionCandidate((*getBrothers())[4]);
	}
	
	if (!executor->collisionMap(0,movy)) 
		executor->m_Scrollxy.y+=movy;
	if (!executor->collisionMap(movx,0)) 
		executor->m_Scrollxy.x+=movx;

	executor->setSprite(sptset[seq],nextStep());
	
}

void CActionWalkAuto0::lastExecute() {
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

bool CActionWalkAuto0::ThisType(const char* i) {
	const char* type[] = { "npc_normal_walk_upleft", "npc_normal_walk_left", "npc_normal_walk_downleft", "npc_normal_walk_up", "npc_normal_walk_down", "npc_normal_walk_upright", "npc_normal_walk_right", "npc_normal_walk_downright" };
	int size = (sizeof(type)/sizeof(type[0]));
	bool ret = false;
	for (int it=0;it<size;it++)
		ret = ret || strcmp(type[it],i)==0;
	return ret;
}

CAction* CActionWalkAuto0::clone(CActorScrollMap* exe) {
	CActionWalkAuto0* ret = new CActionWalkAuto0(exe,NULL,movx,movy,getId());
	cloneThisInto(ret);
	return ret;
}