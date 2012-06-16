#include "IScrollCamera.h"
#include "IScrollCameraObject.h"
#include "IScrollCameraTile.h"

#include "IDebug.h"


IScrollCamera::IScrollCamera(Actor* target, flatscene::Rectangle* area) :
    // Se ha de llamar a la clase Base para una correcta inicializaci�n
    Camera(target,area), centro(new Point(area->getW()/2,area->getH()/2)), objetive("objetivo") {
        intraMargenX=intraMargenY=0;
        loadUniverse();
}

IScrollCamera::~IScrollCamera() {

    delete centro;

}

int IScrollCamera::loadUniverse() {

    if (target->getUniverse() != uni) {
        uni = target->getUniverse();
        CX()=-1000;
        CY()=-1000;
    }

    IScrollLevel* suni = dynamic_cast<IScrollLevel*>(uni);

    if (suni == NULL) 
        return FRACASO;

    objetive.setUniverse(uni);

    auto et = suni->_MA.begin();

    for (auto it = suni->_layerlvl.begin(); it != suni->_layerlvl.end() ; ++it) {

        layerset.push_back( new IScrollCameraTile(it->tiles,&objetive,area));

        if (it->dur && et != suni->_MA.end()) {
            layerset.push_back( new IScrollCameraObject(*et,&objetive,area));
            ++et;
        }
    }

    return EXITO;

}

int IScrollCamera::unloadUniverse() {

    if (uni!=NULL) {

        // TODO

    }

    return EXITO;

}

int IScrollCamera::resyncUniverse() {

    /*		IMPLEMENTACI�N POR DEFECTO	
    /*
    if (unloadUniverse() == EXITO)
    return loadUniverse();
    else
    return FRACASO;
    */

    // TODO

    return EXITO;
}

int IScrollCamera::setTarget(Actor* newTarget) {

    /*		IMPLEMENTACI�N POR DEFECTO */

    /*if (newTarget == this->target) {
    CLibrary::Error("Actor objetivo ya establecido");
    return FRACASO;
    }

    if (newTarget->getUniverse()!= this->target->getUniverse()) {
    this->target=newTarget;
    resyncUniverse();
    } else {
    this->target=newTarget;
    }

    CX()=CY()=-1000; // Truco usado para forzar una recalibraci�n de las coordenadas de la c�mara en su primer uso.
    */

    // TODO

    return EXITO;
}

int IScrollCamera::refresh() {

    if (!isOpened())
        loadUniverse();

    IScrollObject* scrolltarget = (IScrollObject*) target;
    IScrollLevel* map = (IScrollLevel*) uni;

    int mov_x = CX() - scrolltarget->place.x;

    if (abs( mov_x ) > intraMargenX) {
        if ( mov_x < 0 ) {
            CX()=scrolltarget->place.x-intraMargenX;
        } else {
            CX()=scrolltarget->place.x+intraMargenX;
        }

        if ( CX() < centro->getX() + (int) map->getTileW()) { // La camara no enfoca los bordes del map.
            CX() = centro->getX()+ map->getTileW();
        } else 	if ( CX() > (int) (map->getW()*map->getTileW() - area->getW() + centro->getX() - map->getTileW())) {
            // Quizás sería más apropiado restarle adicionalmente 1 unidad a 
            // los margenes exteriores del map (derecha y abajo).
            CX() = map->getW()*map->getTileW() - area->getW() + centro->getX() - map->getTileW() ;
        }
    }

    int mov_y=CY()-scrolltarget->place.y;

    if (abs(mov_y) > intraMargenY) {
        if ( mov_y < 0 ) {
            CY()=scrolltarget->place.y-intraMargenY;
        } else {
            CY()=scrolltarget->place.y+intraMargenY;
        }

        if ( CY() < centro->getY() + (int) map->getTileH() ) {
            CY() = centro->getY() + map->getTileH();
        } else	if ( CY() > (int) (map->getH()*map->getTileH()- area->getH() + centro->getY() - map->getTileH())) {
            CY() = map->getH()*map->getTileH() - area->getH() + centro->getY() - map->getTileH() ;   
        }
    }

    scrolltarget->renderPoint.x = scrolltarget->place.x -(CX() - (area->getW()/2));  
    scrolltarget->renderPoint.y = scrolltarget->place.y -(CY() - (area->getH()/2));

    objetive.renderPoint.x = CX() - centro->getX();
    objetive.renderPoint.y = CY() - centro->getY();

    // GRAPHIC CALLS

    locateRenderScene(area->x*2,area->y*2,area->w*2,area->h*2);

    scale(2.0,2.0,1.0);

    for (auto e = layerset.begin(), f = layerset.end() ; e != f; ++e) {

        (*e)->render();
    }

    return EXITO;
}
