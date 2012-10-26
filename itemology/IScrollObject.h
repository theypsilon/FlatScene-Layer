#ifndef _I_SCROLL_OBJECT_
#define _I_SCROLL_OBJECT_

#include "IDebug.h"

class IScrollObject;

class IScrollObject : public Cinema::Actor {

    std::list<IScrollObject*>* _placeInMA;

    friend class IScrollLevel;

    static int instances;

public:

    Coordinate place;
    // TODO : variables y funciones p�blicas adicionales

    // Constructor con clase menajera como par�metro de entrada 
    IScrollObject();

    // Destructor, ah� deberemos liberar los recursos
    virtual ~IScrollObject();

    // Devuelve un CSprite. Definici�n opcional. Ya tiene la siguiente implementaci�n :
    //
    //	CSprite* CActor::getSprite() {
    //		return CImg.get(file)->get(graph);
    //	}
    //
    // S�lo definir si se desea hacer una operaci�n diferente
    virtual const Sprite* getSprite();

    // M�todo pensado para ser invocado en CEngine::onIdle.
    // Deber� actualizar el estado del CActor cuando sea necesario.
    virtual void move();

    // Opcional. Deber�a devolver una cadena que identificara de alg�n modo al actor.
    // Su implementaci�n por defecto devuelve la cadena "criature" que debemos introducir 
    // en la implementaci�n del Constructor (ver en la parte de implementaci�n).
    virtual const std::string& getCreature();

    // Opcional. Su implementaci�n por defecto es una simple asignaci�n a la variable miembro CUniverse* inUniverse.
    virtual void setUniverse(Cinema::Universe* m);

    // Opcional. Por si se quiere implementar una funci�n que facilite la inicializaci�n de muchos actores dle mismo tipo.
    // Si no se le da una implementaci�n y el m�todo es invocado, devuelve NULL y registra un error.
    virtual Cinema::Actor* clone();


    static int getInstances() { return instances; };
};

#endif
