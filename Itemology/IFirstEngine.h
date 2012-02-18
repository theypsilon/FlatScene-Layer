#ifndef __I_FIRST_ENGINE__
#define __I_FIRST_ENGINE__

#include <FSLibrary.h>

#include "IScrollCamera.h"

class IFirstEngine :
	public CEngine
{
private:
	vector<IScrollCamera*> cams;

	IScrollObject* mainactor;
public:
	// Constructor del Motor.
	IFirstEngine(CMessageHandler* pmhParent=NULL);
	
	// Destructor, ahí deberemos liberar todos los recursos.
	virtual ~IFirstEngine();

	// Opcional, pero invocado por la Libraría. Debe devolver 'true' para que la librería pueda tomar el motor como "Motor Activo" ( CLibrary::getActualEngine(); )
	// En este método se pueden reservar recursos necesarios para la ejecución de lmotor.
	int onInit();
	
	// Opcional, pero invocado por la Librería. En este método se pueden liberar recursos que eran necesarios para la ejecución de lmotor.
	int onExit();

	// Función necesaria para el procesamiento de Eventos por parte de este Motor.
	int loop();

	// Rutina principal. Aquí se debe incluir la implementación principal del motor para actualizar su estado.
	int onIdle();

	// Opcional. Pensado para resolver conflictos ante inesperadas conmutaciones entre diferentes Motores.
	virtual void deselect();

	// Rutina de renderización. Aquí deberían invocarse todas las rutina de renderización de los elementos controlados por el Motor.
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





	// A continuación, se definen 2 métodos hereados de la clase CMessageHandler.
	
	// Dichos métodos también están presentes en CActor, CUniverse y CCamera.

	// Pueden ser redefinidos para gestionar mensajería en el objeto actual.


	// Este método gestiona los mensajes en el instante que se llama a "SendMessage", método definido en CMessageHandler.
	 int onMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2);

	 // Este método gestiona los mensajes no instantáneos. Se leen los mensajes tras llamar al método "readMessages" definido en CMessageHandler.
	 void pendingMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2);
};

#endif