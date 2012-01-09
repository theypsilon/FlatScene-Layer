#include "ActionXPunto.h"
#include "Player.h"

CActionXPunto::CActionXPunto(CActorScrollMap* executor,TiXmlElement* fuente,const char* i) : CAction(executor,fuente,i){ 

	this->num=1;
	this->limitofsequence= alloc(int,this->num);
	this->flags= alloc(int,this->num);
	this->sptset= alloc(int,this->num);
	this->sequence=  alloc(int*,this->num);
	for (int i=0;i<this->num;i++) {
		this->limitofsequence[i]=1;
		this->flags[i] = 0;
		this->sptset[i]=Img.add("Punto");
		this->sequence[i] = alloc(int,1);
		this->sequence[i][0] = 0;
	}

}

CActionXPunto::~CActionXPunto() { 
	inDelete();
}

void CActionXPunto::inDelete() {
	CAction::inDelete();
}

void CActionXPunto::firstExecute() {
	executor->flags = flags[seq];

	paso=0;

}


void CActionXPunto::execute() {

	executor->setSprite(sptset[seq],nextStep());
	
}

void CActionXPunto::lastExecute() {

}