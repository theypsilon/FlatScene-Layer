#include "Action.h"
#include "Player.h"
#include "FSLibrary.h"

CAction::CAction(CActorScrollMap* executor,TiXmlElement* fuente,const char* i) : priority(0), executor(executor), id(i), activo(false), predecesor(NULL), sucesor(NULL), actionBrothers(NULL),
CNode(16), CAnimation() {

    std::string creature = executor->getCreature();
    
    if (fuente != NULL) {
        generateAnimationByXML(fuente,creature.c_str());
    }

}

const char* CAction::getId() { return id.c_str(); }

CAction::~CAction() {
    inDelete();
}

void CAction::inDelete() {
    CNode::inDelete();
}

void CAction::use() { 
    activo=true; 
}

void CAction::recycle() 
{ 
    activo=false; 
}

CAction* CAction::getPredecesor() {
    return predecesor;
}
CAction* CAction::getSucesor() {
    return sucesor;
}
void CAction::setPredecesor(CAction* actPre) {
    predecesor=actPre;
}
void CAction::setSucesor(CAction* actSuc) {
    sucesor=actSuc;
}

CAction* CAction::getKeydown(int n) {
    return ((CAction*)CNode::get(n));
}

CAction* CAction::getKeyup(int n) {
    return ((CAction*)CNode::get(n+8));
}


void CAction::setKeydown(int n, CAction* act) {
    CNode::set(n,(CNode*)act);
}

void CAction::setKeyup(int n,CAction* act) {
    CNode::set(n+8,(CNode*)act);
}

void CAction::firstExecute() {
}

void CAction::execute() {
}

void CAction::lastExecute() {
}

CAction* CAction::clone(CActorScrollMap* exe) {
    FSLib.Error("CAction no implementa un metodo de clonaci�n por defecto.");
    return NULL;
}

CAction* CAction::mix(ActionCollection mix) { return 0; }

