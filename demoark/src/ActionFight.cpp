#include "ActionFight.h"
#include "FSLibrary.h"
#include "Player.h"

#include "ActionComboDemo0.h"
#include "ActionComboDemo1.h"
#include "ActionSimpleDemo0.h"


CActionFight::CActionFight (CActorScrollMap* executor,TiXmlElement* fuente,const char* i) : time1(0), CAction(executor,fuente,i) { 
    idProvisional=atoi(i);
}

CActionFight::~CActionFight() { 
    inDelete();
}

void CActionFight::inDelete() {
    CAction::inDelete();
}

CAction* CActionFight::ActionFightFactory(CActorScrollMap* executor,TiXmlElement* n) {
    const char *i = n->Attribute("id");

    CAction* ret = NULL;
    if (CActionSimpleDemo0::ThisType(i)) {
    	ret = new CActionSimpleDemo0(executor,n,i);
    } else if (strcmp(i,"pj_normal_osa")==0) {
    	ret =  new CActionComboDemo0(executor,n,i);
    } else if (strcmp(i,"pj_normal_osa")==0) {	// TODO: Este debe cambiar.
    	ret =  new CActionComboDemo1(executor,n,i);
    }

    if (ret == NULL) {
    	FSLib.Error(string("No existe una acci�n con esa id ")+i);
    	ret = new CActionSimpleDemo0(executor,n,i);
    }
    return ret;
}
