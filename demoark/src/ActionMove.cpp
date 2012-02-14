#include "ActionWalk0.h"
#include "ActionQuiet0.h"
#include "ActionWalkAuto0.h"
#include "ActionQuietAuto0.h"
#include "Player.h"
#include "FSLibrary.h"

/*
CActionMove::CActionMove(int x, int y,int ppf,int i) : movx(x),movy(y), ppf(ppf), timeOld(0), timeNew(0), MsgID(CPlayer::MSGID_CharSpeed), CAction(i){ 
}*/
CActionMove::CActionMove(CActorScrollMap* executor,TiXmlElement* fuente,int x, int y,const char* i) : movx(x),movy(y), CAction(executor,fuente,i){ 

    if ( x > 0 )
    	dirx=1;
    else if ( x < 0 )
    	dirx=-1;
    else
    	dirx=0;

    if ( y > 0 )
    	diry=1;
    else if ( y < 0 )
    	diry=-1;
    else
    	diry=0;

}

CActionMove::~CActionMove() { 
    inDelete();
}

void CActionMove::inDelete() {
    CAction::inDelete();
}

void CActionMove::use() { 
    timeOld=timeNew;
    timeNew=Chrono.getTick(); 
    CAction::use(); 
}

CAction* CActionMove::mix(ActionCollection mix) { 
    vector<CAction*>* actionBrothers = getBrothers();
    if (actionBrothers) {
    	CActionMove* nextN;
    	int x=getX();
    	int y=getY();
    	for (ActionCollection::iterator it=mix.begin();it!=mix.end();++it) {
    		nextN=(CActionMove*)*it;
    		x+=nextN->getX();
    		y+=nextN->getY();
    	}
    	return (*actionBrothers)[(3*(x+1)+y+1)];
    } else
    	return this;
}

CAction* CActionMove::ActionMoveFactory(CActorScrollMap* executor,TiXmlElement* n) {
    const char* i=n->Attribute("id");
    int x=atoi(n->Attribute("x"));
    int y=atoi(n->Attribute("y"));

    CAction* ret = NULL;
    if (CActionWalk0::ThisType(i)) {
    	ret = new CActionWalk0(executor,n,x,y,i);
    } else if (CActionWalkAuto0::ThisType(i)) {
    	ret = new CActionWalkAuto0(executor,n,x,y,i);
    } else if (strcmp(i,"pj_normal_quiet")==0 || strcmp(i,"testa2animation")==0) {
    	ret = new CActionQuiet0(executor,n,i);
    } else if (strcmp(i,"npc_normal_quiet")==0) {
    	ret = new CActionQuietAuto0(executor,n,i);
    }

    if (ret==NULL) {
    	FSLib.Error(string("No existe una acci�n de movimiento de ese tipo ")+i);
    }
    	
    return ret;
}