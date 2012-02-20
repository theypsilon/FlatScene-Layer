#ifndef _I_SCROLL_OBJECT_
#define _I_SCROLL_OBJECT_

#include "IDebug.h"

#include "FSActor.h"

class IScrollObject;

typedef std::list<IScrollObject*> IScrollObjectCollection;

class IScrollObject :
	public FSActor
{
private:
	IScrollObjectCollection* placeInMA;

	friend class IScrollLevel;

	static int instances;
public:

	FSCoordinate place;
	// TODO : variables y funciones p�blicas adicionales

	// Constructor con clase menajera como par�metro de entrada 
	IScrollObject();

	// Destructor, ah� deberemos liberar los recursos
	~IScrollObject();

	// Devuelve un CSprite. Definici�n opcional. Ya tiene la siguiente implementaci�n :
	//
	//	CSprite* CActor::getSprite() {
	//		return CImg.get(file)->get(graph);
	//	}
	//
	// S�lo definir si se desea hacer una operaci�n diferente
	FSSprite* getSprite();

	// M�todo pensado para ser invocado en CEngine::onIdle.
	// Deber� actualizar el estado del CActor cuando sea necesario.
	int move();

	// Opcional. Deber�a devolver una cadena que identificara de alg�n modo al actor.
	// Su implementaci�n por defecto devuelve la cadena "criature" que debemos introducir 
	// en la implementaci�n del Constructor (ver en la parte de implementaci�n).
	std::string getCreature();
	
	// Opcional. Su implementaci�n por defecto es una simple asignaci�n a la variable miembro CUniverse* inUniverse.
	int setUniverse(FSUniverse* m);

	// Opcional. Por si se quiere implementar una funci�n que facilite la inicializaci�n de muchos actores dle mismo tipo.
	// Si no se le da una implementaci�n y el m�todo es invocado, devuelve NULL y registra un error.
	virtual FSActor* clone();


	static int getInstances() { return instances; };
};

#endif
