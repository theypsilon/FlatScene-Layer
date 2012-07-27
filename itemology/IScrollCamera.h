#ifndef _I_SCROLL_CAMERA_
#define _I_SCROLL_CAMERA_

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
    IScrollCamera(Actor* target,FlatScene::Rectangle* area);

    // Destructor, ah� deberemos liberar los recursos
    virtual ~IScrollCamera();

    // Opcional. Este m�todo est� pensado para usarse cada vez que se enfoca un Universo por primera vez.
    virtual void loadUniverse();

    // Opcional. Este m�todo est� pensado para usarse cuando la c�mara deja de usar un Universo.
    virtual void unloadUniverse();

    // Opcional. Fija el objetivo de la c�mara. La implementaci�n por defecto est� comentada
    // junto al resto de la implementaci�n de la clase.
    virtual void setTarget(Actor* newTarget);

    // M�todo que se llama desde el m�todo de la clase base "render". Se debe encargar de actualizar los elementos gr�ficos.
    virtual void refresh();
};

#endif
