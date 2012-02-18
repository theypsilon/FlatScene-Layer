#ifndef __I_FIRST_ENGINE__
#define __I_FIRST_ENGINE__

#include <FSLibrary.h>

#include "IScrollCamera.h"

class IFirstEngine :
	public FSEngine
{
private:
	std::vector<IScrollCamera*> cams;

	IScrollObject* mainactor;

    FSMultiverse CMultiverse;
public:
	// Constructor del Motor.
	IFirstEngine();
	
	// Destructor, ah� deberemos liberar todos los recursos.
	virtual ~IFirstEngine();

	// Opcional, pero invocado por la Librar�a. Debe devolver 'true' para que la librer�a pueda tomar el motor como "Motor Activo" ( CLibrary::getActualEngine(); )
	// En este m�todo se pueden reservar recursos necesarios para la ejecuci�n de lmotor.
	int onInit();
	
	// Opcional, pero invocado por la Librer�a. En este m�todo se pueden liberar recursos que eran necesarios para la ejecuci�n de lmotor.
	int onExit();

	// Funci�n necesaria para el procesamiento de Eventos por parte de este Motor.
	int loop();

	// Rutina principal. Aqu� se debe incluir la implementaci�n principal del motor para actualizar su estado.
	int onIdle();

	// Opcional. Pensado para resolver conflictos ante inesperadas conmutaciones entre diferentes Motores.
	virtual void deselect();

	// Rutina de renderizaci�n. Aqu� deber�an invocarse todas las rutina de renderizaci�n de los elementos controlados por el Motor.
	int drawFrame();

	void onKeyboard(SDL_Event* e);

	/*
	void onAPush();
	void onBPush();
	void onCPush();
	void onDPush();
	void onEPush();
	void onFPush();

	void onARelease();
	void onBRelease();
	void onCRelease();
	void onDRelease();
	void onERelease();
	void onFRelease();

	void onMoveLPush();
	void onMoveRPush();
	void onMoveUPush();
	void onMoveDPush();

	void onMoveLRelease();
	void onMoveRRelease();
	void onMoveURelease();
	void onMoveDRelease();*/

	enum moving_key {
		i_up,
		i_down,
		i_right,
		i_left,
		i_count
	};

	bool move[i_count];

};

#endif