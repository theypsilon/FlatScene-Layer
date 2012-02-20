#ifndef _I_SCROLL_CAMERA_
#define _I_SCROLL_CAMERA_

#include "FSLibrary.h"
#include "IScrollLevel.h"

class IScrollCamera :
	public FSCamera
{
private:
	FSPoint* centro;
	int intraMargenX, intraMargenY;

	std::vector<FSCamera*> layerset;

	FSActor objetive;

	// TODO : funciones y variables privadas

public:

	// TODO : variables y funciones públicas adicionales

	// Constructor, se recomienda que se establezcan los siguientes parámetros.
	// Pueden ser necesarios para inicializar la clase base.
	IScrollCamera(FSActor* target,FSRectangle* area);

	// Destructor, ahí deberemos liberar los recursos
	virtual ~IScrollCamera();

	// Opcional. Este método está pensado para usarse cada vez que se enfoca un Universo por primera vez.
	int loadUniverse();
	
	// Opcional. Este método está pensado para usarse cuando la cámara deja de usar un Universo.
	int unloadUniverse();

	// Opcional. Este método está pensado para recargar los recursos gráficos provenientes del Universo si fuera necesario.
	int resyncUniverse();
	// La implementación por defecto de los 3 métodos anteriores simplemente devuelve ÉXITO.

	// Opcional. Fija el objetivo de la cámara. La implementación por defecto está comentada
	// junto al resto de la implementación de la clase.
	int setTarget(FSActor* newTarget);

	// Método que se llama desde el método de la clase base "render". Se debe encargar de actualizar los elementos gráficos.
	int refresh();
};

#endif
