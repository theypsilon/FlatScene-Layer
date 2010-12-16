#ifndef __UNIVERSE_TEMPLATE__
#define __UNIVERSE_TEMPLATE__

#include "Universe.h"

class CUniverseTemplate : public CUniverse {
private:

	// TODO : funciones y variables privadas

public:

	// TODO : variables y funciones públicas adicionales

	// Constructor. La cadena name es el descriptor del Universo. Para la gestión de Universos, debería usarse el objeto global CMultiverse.
	// Cuando se crea un Universo, debería añadirse su instancia inmediatamente a CMultiverse mediante CMultiverse.add(CUniverse*)
	CUniverseTemplate(string name);

	// Destructor, ahí deberemos liberar los recursos. Si queremos que sólo pueda invocarse a través de CMultiverse, 
	// se recomienda mover el destructor a la sección 'private', y seguidamente añadir la línea "friend class CControlMultiverse;".
	virtual ~CUniverseTemplate();

	// Opcional. Método dedicado para la inicialización de los recursos del universo.
	void load();

	// Opcional. Método dedicado para la finalización de los recursos del universo.
	void unload();

	// Opcional. Método que sustituya las características de este Universo por las de otro compatible a través de un nuevo descriptor.
	int changeUniverse(string name, Uint8 slot);

	// Opcional. Método para notificación de la entrada de un CActor.
	int incActor(CActor* act);

	// Opcional. Método para notificación de la salida de un CActor.
	int decActor(CActor* act);

	// Opcional. Método para notificación de la entrada de una CCamera.
	void incCameras();

	// Opcional. Método para notificación de la salida de una CCamera.
	void decCameras();

};

#endif