#include "GarbageCollector.h"

CGarbageCollector::CGarbageCollector() {
}
CGarbageCollector::~CGarbageCollector() {
    //clean();
}
void CGarbageCollector::clean() {
}


CGarbageCollectorOfActions::CGarbageCollectorOfActions() {
}

CGarbageCollectorOfActions::~CGarbageCollectorOfActions() {
    clean();
}

void CGarbageCollectorOfActions::clean() {
    for (list<CAction*>::iterator it=nodes.begin();it!=nodes.end();++it)
    	delete (*it);
    nodes.clear();
    for (list<vector<CAction*>*>::iterator it=listOfBrothers.begin();it!=listOfBrothers.end();++it)
    	delete (*it);
    listOfBrothers.clear();
}

void CGarbageCollectorOfActions::add(vector<CAction*>* bro) {
    listOfBrothers.push_back(bro);
}

void CGarbageCollectorOfActions::add(CAction* nod) {
    nodes.push_back(nod);
}

list<CAction*>& CGarbageCollectorOfActions::getListActions() {
    return nodes;
}

list<vector<CAction*>*>& CGarbageCollectorOfActions::getListBrothers() {
    return listOfBrothers;
}