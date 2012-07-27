#include "Camera.h"
#include "ScreenImpl.h"
#include "Universe.h"
#include "Exception.h"
#include "Algorithm.h"

namespace FlatScene {

Camera::Camera(Actor* target, Rectangle* area)
: uni(nullptr), target(target), area(area) , rendering(false), x(-1000),y(-1000) {}

Camera::~Camera() {
    delete area;
}

Universe* Camera::getUniverse() {
    return uni;
}

void Camera::loadUniverse() {
    if (uni != nullptr) throw Exception("Universe already loaded");
}

void Camera::unloadUniverse() {
    if (uni == nullptr) throw Exception("No Universe in focus");

    uni->decCameras();
    uni = nullptr;
}

bool Camera::isOpened() {
    return uni != nullptr;
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

void Camera::setTarget(Actor* newTarget) {
    if (newTarget == this->target) throw Exception("Actor objetivo ya establecido");

    if (newTarget->getUniverse()!= this->target->getUniverse()) {
        this->target=newTarget;
        resyncUniverse(*this);
    } else {
        this->target=newTarget;
    }

    CX() = CY() = -1000; // Usado para forzar una recalibraci�n de las coordenadas de la c�mara en su primer uso.

}

Rectangle* Camera::getArea() {
    return area;
}

void Camera::render() {

    rendering = true;

    call_to_all(initRenderList);
    initRenderList.clear();

    refresh();

    call_to_all(endRenderList);
    endRenderList.clear();

    rendering = false;
}

void Camera::refresh() {}

void Camera::locateRenderScene( Rectangle* areaSc, Float zoom ) {
    locateRenderScene(areaSc->getX(),areaSc->getY(),areaSc->getW(),areaSc->getH(),zoom);
}
void Camera::locateRenderScene( Float posx, Float posy, Float width, Float height, Float zoom ) {

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

}
void Camera::rotate(Float angle, Float x, Float y, Float z) {

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

}
void Camera::translate(Float x, Float y, Float z) {

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

}
void Camera::scale(Float x, Float y, Float z) {

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

}
void Camera::color(Float red, Float green, Float blue, Float alpha) {

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

}
void Camera::color(Color* col, Float alpha) {
    color(((Float)col->getR())/255.0,((Float)col->getG())/255.0,((Float)col->getB())/255.0,alpha);
}

void Camera::reubicate(Rectangle* nArea) {
    if (area == nArea) throw Exception("Area ya establecida");

    delete area;
    area = nArea;

}

#ifdef MENSAJES_MSGIDS
int Camera::SendMessage(Uint32 MsgID,MSGPARM ParmMsg) {
    printf("Camera %d.%d :: ",area->getX(),area->getY());
    return MessageHandler::SendMessage(MsgID,ParmMsg,Parm2);
}
#endif


void resyncUniverse(Camera& cam) {
    cam.unloadUniverse();
    cam.loadUniverse();
}

} // flatscene
