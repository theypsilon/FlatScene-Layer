#include "ActorMouse.h"
#include "FSImages.h"

CActorMouse::CActorMouse(FSMessageHandler * pmhParent) : 
// Se ha de llamar a la clase Base para una correcta inicializaci�n
FSActor("raton",pmhParent) {

    // TODO
    
}
CActorMouse::~CActorMouse() {

    // TODO

}

int CActorMouse::move() {

    // TODO

    return EXITO;
}

std::string CActorMouse::getCreature() {

    // TODO

    return std::string("cadena personalizable");

    // CActor::getCreature() accede al valor que hemos configurado en el Constructor.
}

int CActorMouse::setUniverse(FSUniverse* m) {

    // TODO

    return EXITO;

    // CActor::setUniverse(m) realiza la asignaci�n : inUniverse=m;
}

FSSprite* CActorMouse::getSprite() {

    FSSprite* spt;

    spt = Img.get(file)->get(graph);

    return spt;
}

FSActor* CActorMouse::clone() {
    FSActor* act = NULL;

    // TODO

    return act;
}
