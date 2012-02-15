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
    for (std::list<CAction*>::iterator it=nodes.begin();it!=nodes.end();++it)
        delete (*it);
    nodes.clear();
    for (std::list<std::vector<CAction*>*>::iterator it=listOfBrothers.begin();it!=listOfBrothers.end();++it)
        delete (*it);
    listOfBrothers.clear();
}

void CGarbageCollectorOfActions::add(std::vector<CAction*>* bro) {
    listOfBrothers.push_back(bro);
}

void CGarbageCollectorOfActions::add(CAction* nod) {
    nodes.push_back(nod);
}

std::list<CAction*>& CGarbageCollectorOfActions::getListActions() {
    return nodes;
}

std::list<std::vector<CAction*>*>& CGarbageCollectorOfActions::getListBrothers() {
    return listOfBrothers;
}
