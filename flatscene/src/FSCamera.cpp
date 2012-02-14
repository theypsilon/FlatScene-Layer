#include "FSCamera.h"
#include "FSScreenImpl.h"
#include "FSLibrary.h"

FSCamera::FSCamera(FSActor* target, FSRectangle* area,FSMessageHandler * pmhParent) :
FSMessageHandler(pmhParent), uni(NULL), target(target), area(area) , rendering(false),
x(-1000),y(-1000) {}

FSCamera::~FSCamera() {
    delete area;
}

FSUniverse* FSCamera::getUniverse() {
    return uni;
}

int FSCamera::loadUniverse() {

    if (uni != NULL) {
    	FSLibrary::I().Error("Universe already loaded");
    	return FRACASO;
    }


    return EXITO;
}

int FSCamera::unloadUniverse() {
    if (uni == NULL) {
    	FSLibrary::I().Error("No Universe in focus");
    	return FRACASO;
    }


    uni->decCameras();
    uni=NULL;

    return EXITO;

}

int FSCamera::resyncUniverse() {

    if (unloadUniverse() == EXITO)
    	return loadUniverse();
    else
    	return FRACASO;
}

bool FSCamera::isOpened() {
    return (uni!=NULL);
}

int& FSCamera::CX() {
    return x;
}
int& FSCamera::CY() {
    return y;
}
FSActor* FSCamera::Target() {
    return target;
}

int FSCamera::setTarget(FSActor* newTarget) {
    if (newTarget == this->target) {
    	FSLibrary::I().Error("Actor objetivo ya establecido");
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

FSRectangle* FSCamera::getArea() {
    return area;
}

int FSCamera::render() {

    rendering = true;

    for (list<std::function<void()>>::const_iterator iri = initRenderList.begin(), ire = initRenderList.end();iri!=ire;++iri) {
    	(*iri)();
    }

    initRenderList.clear();

    int ret = refresh();

    for (list<std::function<void()>>::const_iterator eri = endRenderList.begin(), ere = endRenderList.end();eri!=ere;++eri) {
    	(*eri)();
    }

    endRenderList.clear();

    rendering = false;

    return ret;
}

int FSCamera::refresh() {
    return EXITO;
}

int FSCamera::locateRenderScene( FSRectangle* areaSc, float zoom ) {
    return locateRenderScene(areaSc->getX(),areaSc->getY(),areaSc->getW(),areaSc->getH(),zoom);
}
int FSCamera::locateRenderScene( float posx, float posy, float width, float height, float zoom ) {

    if (width == 0 || height == 0) {
    	posx = area->getX();
    	posy = area->getY();
    	width = area->getW();
    	height = area->getH();
    }

    if (rendering) {
    	FSScreen::I().pushMatrix();
    	FSScreen::I().locateRenderScene(posx,posy,width,height,zoom);
    } else {
    	initRenderList.push_back([](){
    		FSScreen::I().pushMatrix();
    	});
    	initRenderList.push_back([=](){
    		FSScreen::I().locateRenderScene(posx,posy,width,height,zoom);
    	});
    }

    endRenderList.push_front([](){
    	FSScreen::I().popMatrix();
    });

    return EXITO;
}
int FSCamera::rotate(float angle, float x, float y, float z) {

    if (rendering) {
    	FSScreen::I().pushMatrix();
    	FSScreen::I().rotate(angle,x,y,z);
    } else {
    	initRenderList.push_back([](){
    		FSScreen::I().pushMatrix();
    	});
    	initRenderList.push_back([=](){
    		FSScreen::I().rotate(angle,x,y,z);
    	});
    }

    endRenderList.push_front([](){
    	FSScreen::I().popMatrix();
    });

    return EXITO;
}
int FSCamera::translate(float x, float y, float z) {

    if (rendering) {
    	FSScreen::I().pushMatrix();
    	FSScreen::I().translate(x,y,z);
    } else {
    	initRenderList.push_back([](){
    		FSScreen::I().pushMatrix();
    	});
    	initRenderList.push_back([=](){
    		FSScreen::I().translate(x,y,z);
    	});
    }

    endRenderList.push_front([](){
    	FSScreen::I().popMatrix();
    });

    return EXITO;
}
int FSCamera::scale(float x, float y, float z) {

    if (rendering) {
    	FSScreen::I().pushMatrix();
    	FSScreen::I().scale(x,y,z);
    } else {
    	initRenderList.push_back([](){
    		FSScreen::I().pushMatrix();
    	});
    	initRenderList.push_back([=](){
    		FSScreen::I().scale(x,y,z);
    	});
    }

    endRenderList.push_front([](){
    	FSScreen::I().popMatrix();
    });

    return EXITO;
}
int FSCamera::color(float red, float green, float blue, float alpha) {

    if (red > 1.0) red = 1.0;
    if (green > 1.0) green = 1.0;
    if (blue > 1.0) blue = 1.0;
    if (alpha > 1.0) alpha = 1.0;

    if (rendering) {
    	FSScreen::I().color(red,green,blue,alpha);
    } else {
    	initRenderList.push_back([=](){
    		FSScreen::I().color(red,green,blue,alpha);
    	});
    }

    red = FSScreen::I()._impl->red;//2.0 - red;
    green = FSScreen::I()._impl->green;//2.0 - green;
    blue = FSScreen::I()._impl->blue;//2.0 - blue;
    alpha =  FSScreen::I()._impl->alpha;//2.0 - alpha;

    endRenderList.push_front([=](){
    	FSScreen::I().color(red,green,blue,alpha);
    });

    return EXITO;
}
int FSCamera::color(FSColor* col, float alpha) {
    return color(((float)col->getR())/255.0,((float)col->getG())/255.0,((float)col->getB())/255.0,alpha);
}

int FSCamera::reubicate(FSRectangle* nArea) {
    if (area == nArea) {
    	FSLibrary::I().Error("Area ya establecida");
    	return FRACASO;
    }

    delete area;
    area=nArea;

    return EXITO;

}

#ifdef MENSAJES_MSGIDS
int FSCamera::SendMessage(Uint32 MsgID,MSGPARM ParmMsg) {
    printf("Camera %d.%d :: ",area->getX(),area->getY());
    return FSMessageHandler::SendMessage(MsgID,ParmMsg,Parm2);
}
#endif
