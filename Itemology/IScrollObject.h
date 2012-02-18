#ifndef _I_SCROLL_OBJECT_
#define _I_SCROLL_OBJECT_

#include "IDebug.h"

#include "fsactor.h"

class IScrollObject;

typedef list<IScrollObject*> IScrollObjectCollection;

class IScrollObject :
	public CActor
{
private:
	IScrollObjectCollection* placeInMA;

	friend class IScrollLevel;

	static int instances;
public:

	CCoordinate place;
	// TODO : variables y funciones públicas adicionales

	// Constructor con clase menajera como parámetro de entrada 
	IScrollObject(CMessageHandler* pmhParent=NULL);

	// Destructor, ahí deberemos liberar los recursos
	~IScrollObject();

	// Devuelve un CSprite. Definición opcional. Ya tiene la siguiente implementación :
	//
	//	CSprite* CActor::getSprite() {
	//		return CImg.get(file)->get(graph);
	//	}
	//
	// Sólo definir si se desea hacer una operación diferente
	CSprite* getSprite();

	// Método pensado para ser invocado en CEngine::onIdle.
	// Deberá actualizar el estado del CActor cuando sea necesario.
	int move();

	// Opcional. Debería devolver una cadena que identificara de algún modo al actor.
	// Su implementación por defecto devuelve la cadena "criature" que debemos introducir 
	// en la implementación del Constructor (ver en la parte de implementación).
	string getCreature();
	
	// Opcional. Su implementación por defecto es una simple asignación a la variable miembro CUniverse* inUniverse.
	int setUniverse(CUniverse* m);

	// Opcional. Por si se quiere implementar una función que facilite la inicialización de muchos actores dle mismo tipo.
	// Si no se le da una implementación y el método es invocado, devuelve NULL y registra un error.
	virtual CActor* clone();


	static int getInstances() { return instances; };
};

#endif