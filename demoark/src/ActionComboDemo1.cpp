#include "ActionComboDemo1.h"
#include "Player.h"


CActionComboDemo1::CActionComboDemo1 (CActorScrollMap* executor,TiXmlElement* fuente,const char* i) : CActionFight(executor,fuente,i) { 
}

CActionComboDemo1::~CActionComboDemo1() { 
    inDelete();
}

void CActionComboDemo1::execute() {
    int id = (int) idProvisional;

    if (id!=16) {
#ifdef MENSAJES_ACCIONES
    	printf("Step%d                                                                     \r",id);
#endif
    	if (elapsed(10)) {
#ifdef MENSAJES_ACCIONES
    		printf("Step%d Time Elapsed          ",id);
#endif
    		executor->removeActionCandidate(this);
    	}
    } else {
#ifdef MENSAJES_ACCIONES
    	printf("Combo Launched!!       ");
#endif
    	executor->removeActionCandidate(this);
    }
}

CAction* CActionComboDemo1::mix(ActionCollection mix) {
    return this;
}

CAction* CActionComboDemo1::clone(CActorScrollMap* exe) {
    if (!exe) exe = executor;
    CActionComboDemo1* ret = new CActionComboDemo1(exe,NULL,getId());
    cloneThisInto(ret);
    return ret;
}
