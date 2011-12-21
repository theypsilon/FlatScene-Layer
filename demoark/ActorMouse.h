#ifndef __CACTOR_MOUSE__
#define __CACTOR_MOUSE__

#include "FSActor.h"


class CActorMouse : public CActor
{
private:

	// TODO : funciones y variables privadas

public:

	// TODO : variables y funciones p�blicas adicionales

	// Constructor con clase menajera como par�metro de entrada 
	CActorMouse(CMessageHandler * pmhParent=NULL);

	// Destructor, ah� deberemos liberar los recursos
	~CActorMouse();

	// Devuelve un CSprite. Definici�n opcional. Ya tiene la siguiente implementaci�n :
	//
	//	CSprite* CActor::getSprite() {
	//		return CImg.get(file)->get(graph);
	//	}
	//
	// S�lo definir si se desea hacer una operaci�n diferente
	CSprite* getSprite();

	// M�todo pensado para ser invocado en CEngine::onIdle.
	// Deber� actualizar el estado del CActor cuando sea necesario.
	int move();

	// Opcional. Deber�a devolver una cadena que identificara de alg�n modo al actor.
	// Su implementaci�n por defecto devuelve la cadena "criature" que debemos introducir 
	// en la implementaci�n del Constructor (ver en la parte de implementaci�n).
	string getCreature();
	
	// Opcional. Su implementaci�n por defecto es una simple asignaci�n a la variable miembro CUniverse* inUniverse.
	int setUniverse(CUniverse* m);

	// Opcional. Por si se quiere implementar una funci�n que facilite la inicializaci�n de muchos actores dle mismo tipo.
	// Si no se le da una implementaci�n y el m�todo es invocado, devuelve NULL y registra un error.
	virtual CActor* clone();

};



#endif