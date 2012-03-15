#include "FSCamera.h"
#include "FSScreenImpl.h"
#include "FSLibrary.h"

namespace flatscene {

Camera::Camera(Actor* target, Rectangle* area)
: uni(NULL), target(target), area(area) , rendering(false), x(-1000),y(-1000) {}

Camera::~Camera() {
    delete area;
}

Universe* Camera::getUniverse() {
    return uni;
}

int Camera::loadUniverse() {

    if (uni != NULL) {
        Library::I().Error("Universe already loaded");
        return FRACASO;
    }


    return EXITO;
}

int Camera::unloadUniverse() {
    if (uni == NULL) {
        Library::I().Error("No Universe in focus");
        return FRACASO;
    }


    uni->decCameras();
    uni=NULL;

    return EXITO;

}

int Camera::resyncUniverse() {

    if (unloadUniverse() == EXITO)
        return loadUniverse();
    else
        return FRACASO;
}

bool Camera::isOpened() {
    return (uni!=NULL);
}

int& Camera::CX() {
    return x;
}
int& Camera::CY() {
    return y;
}
Actor* Camera::Target() {
    return target;
}

int Camera::setTarget(Actor* newTarget) {
    if (newTarget == this->target) {
        Library::I().Error("Actor objetivo ya establecido");
        return FRACASO;
    }

    if (newTarget->getUniverse()!= this->target->getUniverse()) {
        this->target=newTarget;
        resyncUniverse();
    } else {
        this->target=newTarget;
    }

    CX()=CY()=-1000; // Usado para forzar una recalibraci�n de las coordenadas de la c�mara en su primer uso.
    
    return EXITO;
}

Rectangle* Camera::getArea() {
    return area;
}

int Camera::render() {

    rendering = true;

    for (std::list<std::function<void()>>::const_iterator iri = initRenderList.begin(), ire = initRenderList.end();iri!=ire;++iri) {
        (*iri)();
    }

    initRenderList.clear();

    int ret = refresh();

    for (std::list<std::function<void()>>::const_iterator eri = endRenderList.begin(), ere = endRenderList.end();eri!=ere;++eri) {
        (*eri)();
    }

    endRenderList.clear();

    rendering = false;

    return ret;
}

int Camera::refresh() {
    return EXITO;
}

int Camera::locateRenderScene( Rectangle* areaSc, Float zoom ) {
    return locateRenderScene(areaSc->getX(),areaSc->getY(),areaSc->getW(),areaSc->getH(),zoom);
}
int Camera::locateRenderScene( Float posx, Float posy, Float width, Float height, Float zoom ) {

    if (width == 0 || height == 0) {
        posx = area->getX();
        posy = area->getY();
        width = area->getW();
        height = area->getH();
    }

    if (rendering) {
        Screen::I().pushMatrix();
        Screen::I().locateRenderScene(posx,posy,width,height,zoom);
    } else {
        initRenderList.push_back([](){
            Screen::I().pushMatrix();
        });
        initRenderList.push_back([=](){
            Screen::I().locateRenderScene(posx,posy,width,height,zoom);
        });
    }

    endRenderList.push_front([](){
        Screen::I().popMatrix();
    });

    return EXITO;
}
int Camera::rotate(Float angle, Float x, Float y, Float z) {

    if (rendering) {
        Screen::I().pushMatrix();
        Screen::I().rotate(angle,x,y,z);
    } else {
        initRenderList.push_back([](){
            Screen::I().pushMatrix();
        });
        initRenderList.push_back([=](){
            Screen::I().rotate(angle,x,y,z);
        });
    }

    endRenderList.push_front([](){
        Screen::I().popMatrix();
    });

    return EXITO;
}
int Camera::translate(Float x, Float y, Float z) {

    if (rendering) {
        Screen::I().pushMatrix();
        Screen::I().translate(x,y,z);
    } else {
        initRenderList.push_back([](){
            Screen::I().pushMatrix();
        });
        initRenderList.push_back([=](){
            Screen::I().translate(x,y,z);
        });
    }

    endRenderList.push_front([](){
        Screen::I().popMatrix();
    });

    return EXITO;
}
int Camera::scale(Float x, Float y, Float z) {

    if (rendering) {
        Screen::I().pushMatrix();
        Screen::I().scale(x,y,z);
    } else {
        initRenderList.push_back([](){
            Screen::I().pushMatrix();
        });
        initRenderList.push_back([=](){
            Screen::I().scale(x,y,z);
        });
    }

    endRenderList.push_front([](){
        Screen::I().popMatrix();
    });

    return EXITO;
}
int Camera::color(Float red, Float green, Float blue, Float alpha) {

    if (red > 1.0) red = 1.0;
    if (green > 1.0) green = 1.0;
    if (blue > 1.0) blue = 1.0;
    if (alpha > 1.0) alpha = 1.0;

    if (rendering) {
        Screen::I().color(red,green,blue,alpha);
    } else {
        initRenderList.push_back([=](){
            Screen::I().color(red,green,blue,alpha);
        });
    }

    red = Screen::I()._impl->red;//2.0 - red;
    green = Screen::I()._impl->green;//2.0 - green;
    blue = Screen::I()._impl->blue;//2.0 - blue;
    alpha =  Screen::I()._impl->alpha;//2.0 - alpha;

    endRenderList.push_front([=](){
        Screen::I().color(red,green,blue,alpha);
    });

    return EXITO;
}
int Camera::color(Color* col, Float alpha) {
    return color(((Float)col->getR())/255.0,((Float)col->getG())/255.0,((Float)col->getB())/255.0,alpha);
}

int Camera::reubicate(Rectangle* nArea) {
    if (area == nArea) {
        Library::I().Error("Area ya establecida");
        return FRACASO;
    }

    delete area;
    area=nArea;

    return EXITO;

}

#ifdef MENSAJES_MSGIDS
int Camera::SendMessage(Uint32 MsgID,MSGPARM ParmMsg) {
    printf("Camera %d.%d :: ",area->getX(),area->getY());
    return MessageHandler::SendMessage(MsgID,ParmMsg,Parm2);
}
#endif

} // flatscene
