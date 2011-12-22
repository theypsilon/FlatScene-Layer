#include "CameraOS.h"
#include "FSLibrary.h"

// Library.h es necesario para el manejo de errores.

CCameraOS::CCameraOS(FSActor* target, FSRectangle* area,FSMessageHandler * pmhParent) :
// Se ha de llamar a la clase Base para una correcta inicialización
FSCamera(target,area,pmhParent) {
	
	// TODO


	file = 0;
	graph = 0;

}

CCameraOS::~CCameraOS() {
	
	// TODO

}

int CCameraOS::loadUniverse() {

	if (uni!=NULL) {

	// TODO

	}

	return EXITO;

}

int CCameraOS::unloadUniverse() {

	if (uni!=NULL) {

	// TODO

	}

	return EXITO;

}

int CCameraOS::resyncUniverse() {

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

int CCameraOS::setTarget(FSActor* newTarget) {

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

int CCameraOS::refresh() {

	if (target) {

		FSPoint p = target->renderPoint;

		target->getSprite()->put(p);

	}

	// TODO : Aquí debería ir las rutinas que invocan el proceso de renderización de cada elemento dentro del contexto de la cámara.

	return EXITO;
}
