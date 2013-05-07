#ifndef CINEMA_CAMERA_IMPL__
#define CINEMA_CAMERA_IMPL__

#include "Camera.h"
#include "Universe.h"
#include "Exception.h"
#include "Algorithm.h"

namespace Cinema {

    inline Camera::Camera(Actor* target, FlatScene::Rectangle area)
    : uni(nullptr), target(target), area(std::move(area)) , rendering(false), x(-1000),y(-1000) {}

    inline Universe& Camera::getUniverse() {
        return *uni;
    }

    inline void Camera::loadUniverse() {
        if (uni != nullptr) throw FlatScene::Exception("Universe already loaded");
    }

    inline void Camera::unloadUniverse() {
        if (uni == nullptr) throw FlatScene::Exception("No Universe in focus");

        uni->decCameras();
        uni = nullptr;
    }

    inline bool Camera::isOpened() const {
        return uni != nullptr;
    }

    inline int& Camera::CX() {
        return x;
    }

    inline int& Camera::CY() {
        return y;
    }

    inline Actor& Camera::Target() {
        return *target;
    }

    inline void Camera::setTarget(Actor* newTarget) {
        if (newTarget == this->target) throw FlatScene::Exception("Actor objetivo ya establecido");

        if (std::addressof(newTarget->getUniverse()) != 
            std::addressof(this->target->getUniverse())
        ) {
            this->target=newTarget;
            resyncUniverse(*this);
        } else {
            this->target=newTarget;
        }

        CX() = CY() = -1000; // Usado para forzar una recalibraci�n de las coordenadas de la c�mara en su primer uso.

    }

    inline const FlatScene::Rectangle& Camera::getArea() const {
        return area;
    }

    inline void Camera::render() {

        rendering = true;

        FlatScene::call_to_all(initRenderList);
        initRenderList.clear();

        refresh();

        FlatScene::call_to_all(endRenderList);
        endRenderList.clear();

        rendering = false;
    }

    inline void Camera::refresh() {}

    inline void Camera::locateRenderScene( FlatScene::Rectangle* areaSc, Float zoom ) {
        locateRenderScene(areaSc->getX(),areaSc->getY(),areaSc->getW(),areaSc->getH(),zoom);
    }

    inline void Camera::locateRenderScene( Float posx, Float posy, Float width, Float height, Float zoom ) {

        if (width == 0 || height == 0) {
            posx = area.getX();
            posy = area.getY();
            width = area.getW();
            height = area.getH();
        }

        if (rendering) {
            FlatScene::Screen::I().pushMatrix();
            FlatScene::Screen::I().locateRenderScene(posx,posy,width,height,zoom);
        } else {
            initRenderList.push_back([](){
                FlatScene::Screen::I().pushMatrix();
            });
            initRenderList.push_back([=](){
                FlatScene::Screen::I().locateRenderScene(posx,posy,width,height,zoom);
            });
        }

        endRenderList.push_front([](){
            FlatScene::Screen::I().popMatrix();
        });

    }

    inline void Camera::rotate(Float angle, Float x, Float y, Float z) {

        if (rendering) {
            FlatScene::Screen::I().pushMatrix();
            FlatScene::Screen::I().rotate(angle,x,y,z);
        } else {
            initRenderList.push_back([](){
                FlatScene::Screen::I().pushMatrix();
            });
            initRenderList.push_back([=](){
                FlatScene::Screen::I().rotate(angle,x,y,z);
            });
        }

        endRenderList.push_front([](){
            FlatScene::Screen::I().popMatrix();
        });

    }

    inline void Camera::translate(Float x, Float y, Float z) {

        if (rendering) {
            FlatScene::Screen::I().pushMatrix();
            FlatScene::Screen::I().translate(x,y,z);
        } else {
            initRenderList.push_back([](){
                FlatScene::Screen::I().pushMatrix();
            });
            initRenderList.push_back([=](){
                FlatScene::Screen::I().translate(x,y,z);
            });
        }

        endRenderList.push_front([](){
            FlatScene::Screen::I().popMatrix();
        });

    }

    inline void Camera::scale(Float x, Float y, Float z) {

        if (rendering) {
            FlatScene::Screen::I().pushMatrix();
            FlatScene::Screen::I().scale(x,y,z);
        } else {
            initRenderList.push_back([](){
                FlatScene::Screen::I().pushMatrix();
            });
            initRenderList.push_back([=](){
                FlatScene::Screen::I().scale(x,y,z);
            });
        }

        endRenderList.push_front([](){
            FlatScene::Screen::I().popMatrix();
        });

    }

    inline void Camera::color(Float red, Float green, Float blue, Float alpha) {

        if (red   > 1.0) red   = 1.0;
        if (green > 1.0) green = 1.0;
        if (blue  > 1.0) blue  = 1.0;
        if (alpha > 1.0) alpha = 1.0;

        if (rendering) {
            FlatScene::Screen::I().color(red,green,blue,alpha);
        } else {
            initRenderList.push_back([=](){
                FlatScene::Screen::I().color(red,green,blue,alpha);
            });
        }

        red = FlatScene::Screen::I().getR();//2.0 - red;
        green = FlatScene::Screen::I().getG();//2.0 - green;
        blue = FlatScene::Screen::I().getB();//2.0 - blue;
        alpha =  FlatScene::Screen::I().getA();//2.0 - alpha;

        endRenderList.push_front([=](){
            FlatScene::Screen::I().color(red,green,blue,alpha);
        });

    }

    inline void Camera::color(const FlatScene::Color& col, Float alpha) {
        color(((Float)col.getR())/255.0,((Float)col.getG())/255.0,((Float)col.getB())/255.0,alpha);
    }

    inline void Camera::reubicate(const FlatScene::Rectangle& nArea) {
        area = nArea;
    }

    #ifdef MENSAJES_MSGIDS
    inline int Camera::SendMessage(Uint32 MsgID,MSGPARM ParmMsg) {
        printf("Camera %d.%d :: ",area.getX(),area.getY());
        return MessageHandler::SendMessage(MsgID,ParmMsg,Parm2);
    }
    #endif


    inline void resyncUniverse(Camera& cam) {
        cam.unloadUniverse();
        cam.loadUniverse();
    }
} // Cinema

#endif //CINEMA_CAMERA_IMPL__