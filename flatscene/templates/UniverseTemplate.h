#ifndef __UNIVERSE_TEMPLATE__
#define __UNIVERSE_TEMPLATE__

#include "Universe.h"

class CUniverseTemplate : public Universe {
private:

	// TODO : funciones y variables privadas

public:

	// TODO : variables y funciones p�blicas adicionales

	// Constructor. La cadena name es el descriptor del Universo. Para la gesti�n de Universos, deber�a usarse el objeto global CMultiverse.
	// Cuando se crea un Universo, deber�a a�adirse su instancia inmediatamente a CMultiverse mediante CMultiverse.add(CUniverse*)
	CUniverseTemplate(string name);

	// Destructor, ah� deberemos liberar los recursos. Si queremos que s�lo pueda invocarse a trav�s de CMultiverse, 
	// se recomienda mover el destructor a la secci�n 'private', y seguidamente a�adir la l�nea "friend class CControlMultiverse;".
	virtual ~CUniverseTemplate();

	// Opcional. M�todo dedicado para la inicializaci�n de los recursos del universo.
	void load();

	// Opcional. M�todo dedicado para la finalizaci�n de los recursos del universo.
	void unload();

	// Opcional. M�todo que sustituya las caracter�sticas de este Universo por las de otro compatible a trav�s de un nuevo descriptor.
	int changeUniverse(string name, Uint8 slot);

	// Opcional. M�todo para notificaci�n de la entrada de un CActor.
	int incActor(Actor* act);

	// Opcional. M�todo para notificaci�n de la salida de un CActor.
	int decActor(Actor* act);

	// Opcional. M�todo para notificaci�n de la entrada de una CCamera.
	void incCameras();

	// Opcional. M�todo para notificaci�n de la salida de una CCamera.
	void decCameras();

};

#endif