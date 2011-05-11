#include "ActionComboDemo0.h"
#include "Player.h"


CActionComboDemo0::CActionComboDemo0 (CActorScrollMap* executor,TiXmlElement* fuente,const char* i) : CActionFight(executor,fuente,i) { 
}

CActionComboDemo0::~CActionComboDemo0() { 
	inDelete();
}

void CActionComboDemo0::execute() {
#ifdef MENSAJES_ACCIONES
	printf("Doble acción!    ");
#endif
	executor->removeActionCandidate((*getBrothers())[1]);
	executor->removeActionCandidate((*getBrothers())[2]);
	executor->removeActionCandidate(this);
}

CAction* CActionComboDemo0::mix(ActionCollection mix) {
	return this;
}

CAction* CActionComboDemo0::clone(CActorScrollMap* exe) {
	if (!exe)	exe = executor;
	CActionComboDemo0* ret = new CActionComboDemo0(exe,NULL,getId());
	cloneThisInto(ret);
	return ret;
}
