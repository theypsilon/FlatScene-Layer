#include "EnemyNPC.h"
#include "FSScreen.h"

CEnemyNPC::CEnemyNPC(FSMessageHandler * pmhParent) : CEnemy("E0",pmhParent) {
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

void CEnemyNPC::draw(FSPoint& offset) {

    FSColor c = FSColor::White();

    getSprite()->color(&c,0.5);

    CActorScrollMap::draw(offset);

}
