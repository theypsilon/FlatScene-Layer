#include "EnemyPunto.h"
#include "FSLibrary.h"

CEnemyPunto::CEnemyPunto(FSMessageHandler * pmhParent) : CEnemy("EPUNTO",pmhParent) {
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

FSActor* CEnemyPunto::clone() {

    FSActor* ret = CEnemy::Factory("EPUNTO",getParent());

    std::list<string> none;

    ((CEnemy*)ret)->init(none,0,0,0);

    return ret;

}

