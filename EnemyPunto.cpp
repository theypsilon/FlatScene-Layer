#include "EnemyPunto.h"
#include "Library.h"

CEnemyPunto::CEnemyPunto(CMessageHandler* pmhParent) : CEnemy("EPUNTO",pmhParent) {
	m_Scrollxy.set(100,100,0);
}

CEnemyPunto::~CEnemyPunto() {
	if (garbage) {
		delete garbage;
		garbage=NULL;
	}
	if (rutinaColision) {
		delete rutinaColision;
		rutinaColision=NULL;
	}
}

void CEnemyPunto::init(list<string>& activationIds,int x, int y,int z) {
#ifdef LOG_JUGADORES
	printf("Iniciando enemigo...\n");
#endif
	m_Scrollxy.set(x,y,z);

	actNeutro = new CActionXPunto((CActorScrollMap*)this,NULL,"puntoaccion");

	addActionCandidate(actNeutro);
}

CActor* CEnemyPunto::clone() {

	CActor* ret = CEnemy::Factory("EPUNTO",getParent());

	std::list<string> none;

	((CEnemy*)ret)->init(none,0,0,0);

	return ret;

}

