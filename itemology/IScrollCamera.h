#ifndef _I_SCROLL_CAMERA_
#define _I_SCROLL_CAMERA_

#include "FSLibrary.h"
#include "IScrollLevel.h"

class IScrollCamera :
	public Camera
{
private:
	Point* centro;
	int intraMargenX, intraMargenY;

	std::vector<Camera*> layerset;

	Actor objetive;

	// TODO : funciones y variables privadas

public:

	// TODO : variables y funciones p�blicas adicionales

	// Constructor, se recomienda que se establezcan los siguientes par�metros.
	// Pueden ser necesarios para inicializar la clase base.
	IScrollCamera(Actor* target,flatscene::Rectangle* area);

	// Destructor, ah� deberemos liberar los recursos
	virtual ~IScrollCamera();

	// Opcional. Este m�todo est� pensado para usarse cada vez que se enfoca un Universo por primera vez.
	int loadUniverse();
	
	// Opcional. Este m�todo est� pensado para usarse cuando la c�mara deja de usar un Universo.
	int unloadUniverse();

	// Opcional. Este m�todo est� pensado para recargar los recursos gr�ficos provenientes del Universo si fuera necesario.
	int resyncUniverse();
	// La implementaci�n por defecto de los 3 m�todos anteriores simplemente devuelve �XITO.

	// Opcional. Fija el objetivo de la c�mara. La implementaci�n por defecto est� comentada
	// junto al resto de la implementaci�n de la clase.
	int setTarget(Actor* newTarget);

	// M�todo que se llama desde el m�todo de la clase base "render". Se debe encargar de actualizar los elementos gr�ficos.
	int refresh();
};

#endif
