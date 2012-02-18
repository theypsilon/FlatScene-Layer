#include "IScrollCamera.h"
#include "IScrollCameraObject.h"
#include "IScrollCameraTile.h"

#include "IDebug.h"


IScrollCamera::IScrollCamera(CActor* target, CRectangle* area,CMessageHandler* pmhParent) :
// Se ha de llamar a la clase Base para una correcta inicialización
CCamera(target,area,pmhParent), centro(new CPoint(area->getW()/2,area->getH()/2)), objetive("objetivo",this) {
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

	auto et = suni->MA.begin();

	for (auto it = suni->layerlvl.begin(); it != suni->layerlvl.end() ; ++it) {

		layerset.push_back( new IScrollCameraTile(it->tile,&objetive,area,this));

		if (it->dur && et != suni->MA.end()) {
			layerset.push_back( new IScrollCameraObject(*et,&objetive,area,this));
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

	/*		IMPLEMENTACIÓN POR DEFECTO	
	/*
	if (unloadUniverse() == EXITO)
		return loadUniverse();
	else
		return FRACASO;
	*/

	// TODO

	return EXITO;
}

int IScrollCamera::setTarget(CActor* newTarget) {

	/*		IMPLEMENTACIÓN POR DEFECTO */
	
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

	CX()=CY()=-1000; // Truco usado para forzar una recalibración de las coordenadas de la cámara en su primer uso.
	*/

	// TODO
	
	return EXITO;
}

int IScrollCamera::refresh() {

	if (!isOpened())
		loadUniverse();

	IScrollObject* scrolltarget = (IScrollObject*) target;
	IScrollLevel* map = (IScrollLevel*) uni;

	int mov_x = CX() - scrolltarget->place.X();

	if (abs( mov_x ) > intraMargenX) {
		if ( mov_x < 0 ) {
			CX()=scrolltarget->place.X()-intraMargenX;
		} else {
			CX()=scrolltarget->place.X()+intraMargenX;
		}
		
		if ( CX() < centro->getX() + map->getTileW()) { // La camara no enfoca los bordes del map.
			CX() = centro->getX()+ map->getTileW();
		} else 	if ( CX() > map->getW()*map->getTileW() - area->getW() + centro->getX() - map->getTileW()) { // Quizás sería más apropiado restarle adicionalmente 1 unidad a los margenes exteriores del map (derecha y abajo).
			CX() = map->getW()*map->getTileW() - area->getW() + centro->getX() - map->getTileW() ;
		}
	}

	int mov_y=CY()-scrolltarget->place.Y();

	if (abs(mov_y) > intraMargenY) {
		if ( mov_y < 0 ) {
			CY()=scrolltarget->place.Y()-intraMargenY;
		} else {
			CY()=scrolltarget->place.Y()+intraMargenY;
		}
		
		if ( CY() < centro->getY() + map->getTileH() ) {
			CY() = centro->getY() + map->getTileH();
		} else	if ( CY() >  map->getH()*map->getTileH()- area->getH() + centro->getY() - map->getTileH()) {
			CY() = map->getH()*map->getTileH() - area->getH() + centro->getY() - map->getTileH() ;   
		}
	}

	scrolltarget->renderPoint.X() = scrolltarget->place.X() -(CX() - (area->getW()/2));  
    scrolltarget->renderPoint.Y() = scrolltarget->place.Y() -(CY() - (area->getH()/2));

	objetive.renderPoint.X() = CX() - centro->getX();
	objetive.renderPoint.Y() = CY() - centro->getY();

	// GRAPHIC CALLS

	locateRenderScene(area->X()*2,area->Y()*2,area->W()*2,area->H()*2);

	scale(2.0,2.0,1.0);

	for (auto e = layerset.begin(), f = layerset.end() ; e != f; ++e) {

		(*e)->render();
	}

	return EXITO;
}
