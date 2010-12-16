#include "CameraTemplate.h"
#include "Library.h"

// Library.h es necesario para el manejo de errores.

CCameraTemplate::CCameraTemplate(CActor* target, CRectangle* area,CMessageHandler* pmhParent) :
// Se ha de llamar a la clase Base para una correcta inicialización
CCamera(target,area,pmhParent) {
	
	// TODO

}

CCameraTemplate::~CCameraTemplate() {
	
	// TODO

}

int CCameraTemplate::loadUniverse() {

	if (uni!=NULL) {

	// TODO

	}

	return EXITO;

}

int CCameraTemplate::unloadUniverse() {

	if (uni!=NULL) {

	// TODO

	}

	return EXITO;

}

int CCameraTemplate::resyncUniverse() {

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

int CCameraTemplate::setTarget(CActor* newTarget) {

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

int CCameraTemplate::refresh() {

	// TODO : Aquí debería ir las rutinas que invocan el proceso de renderización de cada elemento dentro del contexto de la cámara.

	return EXITO;
}
