#include "Enemy.h"
#include "FSLibrary.h"
#include "TestAGameInterface.h"

CEnemy::CEnemy(const char* creature,CTestAGameInterface& game) : CActorScrollMap(creature,game) {
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

void CEnemy::init(std::list<std::string>& activationIds,int x, int y,int z) {
#ifdef LOG_JUGADORES
    printf("Iniciando enemigo...\n");
#endif
    m_Scrollxy.set(x,y,z);

    CActorScrollMap::init(activationIds);

}

void CEnemy::damage(CActorScrollMap* executor) {
    game.killEnemy(this,executor,this->getUniverse());
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

    std::string s;
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

FSActor* CEnemy::clone() {
    CEnemy* cloneEnemy = Factory(getCreature().c_str(),game);

    std::list<CAction*>& nodes = this->garbage->getListActions();
    std::list<std::vector<CAction*>*>& listOfBrothers = this->garbage->getListBrothers();

    std::map<CAction*,CAction*> nodesNew;

    cloneEnemy->garbage = new CGarbageCollectorOfActions();

    for (std::list<CAction*>::iterator it=nodes.begin();it!=nodes.end();++it) {
        CAction* cloneAction = (*it)->clone(cloneEnemy);
        if (nodesNew.find(*it)!=nodesNew.end())
            FSLib.Error("Conflicto clonando el CEnemy.");
        nodesNew[*it]=cloneAction;
        cloneEnemy->garbage->add(cloneAction);
    }

    for (std::list<std::vector<CAction*>*>::iterator it=listOfBrothers.begin();it!=listOfBrothers.end();++it) {
        std::vector<CAction*>* brothers = *it;
        int size = brothers->size();
        std::vector<CAction*>* newBrothers = new std::vector<CAction*>;
        for (int i = 0 ; i < size;i++) {
            if (nodesNew.find((*brothers)[i])==nodesNew.end())
                FSLib.Error("Conflicto clonando el CEnemy.");
            newBrothers->push_back(nodesNew[(*brothers)[i]]);
            nodesNew[(*brothers)[i]]->setBrothers(newBrothers);
        }
        cloneEnemy->garbage->add(newBrothers);
    }

    nodesNew[NULL]=NULL;

    for (std::map<CAction*,CAction*>::iterator it=nodesNew.begin();it!=nodesNew.end();++it) {
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

CEnemy* CEnemy::Factory(const char* creature,CTestAGameInterface& game) {
    if (strcmp(creature,"E0")==0) {
        return new CEnemyNPC(game);
    } else if (strcmp(creature,"EPUNTO")==0) {
        return new CEnemyPunto(game);
    }
    FSLib.Error("Factoria CEnemy fallo");
    return NULL;
}
