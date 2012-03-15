#include "CameraTemplate.h"
#include "Library.h"

// Library.h es necesario para el manejo de errores.

CCameraTemplate::CCameraTemplate(Actor* target, Rectangle* area,MessageHandler* pmhParent) :
// Se ha de llamar a la clase Base para una correcta inicializaci�n
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

int CCameraTemplate::setTarget(Actor* newTarget) {

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

int CCameraTemplate::refresh() {

	// TODO : Aqu� deber�a ir las rutinas que invocan el proceso de renderizaci�n de cada elemento dentro del contexto de la c�mara.

	return EXITO;
}
