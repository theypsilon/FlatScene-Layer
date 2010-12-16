#include "EnemyNPC.h"
#include "Screen.h"

CEnemyNPC::CEnemyNPC(CMessageHandler* pmhParent) : CEnemy("E0",pmhParent) {
	m_Scrollxy.set(100,100,0);
}

CEnemyNPC::~CEnemyNPC() {
	if (garbage) {
		delete garbage;
		garbage=NULL;
	}
	if (rutinaColision) {
		delete rutinaColision;
		rutinaColision=NULL;
	}
}

void CEnemyNPC::draw(CPoint& offset) {

	CColor c = CColor::White();

	getSprite()->color(&c,0.5);

	CActorScrollMap::draw(offset);

}