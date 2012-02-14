#include "Enemy.h"
#include "FSLibrary.h"
#include "TestAGameInterface.h"

CEnemy::CEnemy(const char* creature,FSMessageHandler * pmhParent) : CActorScrollMap(creature,pmhParent) {
    m_Scrollxy.set(100,100,0);
}

CEnemy::~CEnemy() {
    if (garbage) {
    	delete garbage;
    	garbage=NULL;
    }
    if (rutinaColision) {
    	delete rutinaColision;
    	rutinaColision=NULL;
    }
}

void CEnemy::init(list<string>& activationIds,int x, int y,int z) {
#ifdef LOG_JUGADORES
    printf("Iniciando enemigo...\n");
#endif
    m_Scrollxy.set(x,y,z);

    CActorScrollMap::init(activationIds);

}

int CEnemy::move() {
    int ret = CActorScrollMap::move();

    if (actActual!=actUltimo) {
    	if (actUltimo)
    		actUltimo->lastExecute();
    	actActual-> firstExecute();
    	actUltimo = actActual;
    }

    actActual->execute();

#ifdef MENSAJES_ACCIONES
    printf("                                                                     \r");

    string s;
    if (actActual) {
    	s = actActual->getId();
    	printf("aA=%s ",s.substr(s.size()-6,s.size()).c_str());
    }
    if (actUltimo) {
    	s = actUltimo->getId();
    	printf("aU=%s ",s.substr(s.size()-6,s.size()).c_str());
    }
    if (actNeutro) {
    	s = actNeutro->getId();
    	printf("aN=%s ",s.substr(s.size()-6,s.size()).c_str());
    }
    if (actUltimoMovimiento) {
    	s = actUltimoMovimiento->getId();
    	printf("aUM=%s ",s.substr(s.size()-6,s.size()).c_str());
    }
    printf("\r");
#endif

    return ret;
}

int CEnemy::onMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2) {
    if (MsgID == CActorScrollMap::MSGID_Damage) {
    	//CActor* hostil = (CActor*) Parm1;

    	void** parm = new void*[2];
    	parm[0] = (void*)getUniverse();
    	parm[1] = Parm1;
    	getParent()->SendMessage(CTestAGameInterface::MSGID_KillEnemy,(MSGPARM)this,(MSGPARM)parm);
    	return EXITO;
    } else
    	return FSMessageHandler::onMessage(MsgID,Parm1,Parm2);
}

FSActor* CEnemy::clone() {
    CEnemy* cloneEnemy = Factory(getCreature().c_str(),getParent());

    list<CAction*>& nodes = this->garbage->getListActions();
    list<vector<CAction*>*>& listOfBrothers = this->garbage->getListBrothers();

    map<CAction*,CAction*> nodesNew;

    cloneEnemy->garbage = new CGarbageCollectorOfActions();

    for (list<CAction*>::iterator it=nodes.begin();it!=nodes.end();++it) {
    	CAction* cloneAction = (*it)->clone(cloneEnemy);
    	if (nodesNew.find(*it)!=nodesNew.end())
    		FSLib.Error("Conflicto clonando el CEnemy.");
    	nodesNew[*it]=cloneAction;
    	cloneEnemy->garbage->add(cloneAction);
    }

    for (list<vector<CAction*>*>::iterator it=listOfBrothers.begin();it!=listOfBrothers.end();++it) {
    	vector<CAction*>* brothers = *it;
    	int size = brothers->size();
    	vector<CAction*>* newBrothers = new vector<CAction*>;
    	for (int i = 0 ; i < size;i++) {
    		if (nodesNew.find((*brothers)[i])==nodesNew.end())
    			FSLib.Error("Conflicto clonando el CEnemy.");
    		newBrothers->push_back(nodesNew[(*brothers)[i]]);
    		nodesNew[(*brothers)[i]]->setBrothers(newBrothers);
    	}
    	cloneEnemy->garbage->add(newBrothers);
    }

    nodesNew[NULL]=NULL;

    for (map<CAction*,CAction*>::iterator it=nodesNew.begin();it!=nodesNew.end();++it) {
    	if (it->first && it->second) {		
    		for (int i=0;i<9;i++) {
    			it->second->setKeydown(i,nodesNew[it->second->getKeydown(i)]);
    			it->second->setKeyup(i,nodesNew[it->second->getKeyup(i)]);
    		}
    	} else if (it->first!=it->second) {
    		FSLib.Error("Conflicto clonando el CEnemy.");
    	}
    }

    cloneEnemy->actActual = nodesNew[this->actActual];
    cloneEnemy->addActionCandidate(nodesNew[this->actNeutro]);
    cloneEnemy->actUltimo = nodesNew[this->actUltimo];
    cloneEnemy->actUltimoMovimiento = nodesNew[this->actUltimoMovimiento];

    cloneEnemy->file = this->file;
    cloneEnemy->graph = this->graph;
    cloneEnemy->flags = this->flags;

    return cloneEnemy;
}

#include "EnemyNPC.h"
#include "EnemyPunto.h"

CEnemy* CEnemy::Factory(const char* creature,FSMessageHandler * pmhParent) {
    if (strcmp(creature,"E0")==0) {
    	return new CEnemyNPC(pmhParent);
    } else if (strcmp(creature,"EPUNTO")==0) {
    	return new CEnemyPunto(pmhParent);
    }
    FSLib.Error("Factoria CEnemy fallo");
    return NULL;
}
