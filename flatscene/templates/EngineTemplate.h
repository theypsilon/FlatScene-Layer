#ifndef __ENGINE_TEMPLATE__
#define __ENGINE_TEMPLATE__

#include "Engine.h"

class CEngineTemplate : public Engine
{
public:

	// Constructor del Motor.
	CEngineTemplate(MessageHandler* pmhParent=NULL);
	
	// Destructor, ah� deberemos liberar todos los recursos.
	virtual ~CEngineTemplate();

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



	// A continuaci�n, se definen 2 m�todos hereados de la clase CMessageHandler.
	
	// Dichos m�todos tambi�n est�n presentes en CActor, CUniverse y CCamera.

	// Pueden ser redefinidos para gestionar mensajer�a en el objeto actual.


	// Este m�todo gestiona los mensajes en el instante que se llama a "SendMessage", m�todo definido en CMessageHandler.
	 int onMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2);

	 // Este m�todo gestiona los mensajes no instant�neos. Se leen los mensajes tras llamar al m�todo "readMessages" definido en CMessageHandler.
	 void pendingMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2);
};


#endif