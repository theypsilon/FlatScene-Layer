#include "EngineTemplate.h"

CEngineTemplate::CEngineTemplate(FSMessageHandler* pmhParent) : FSEngine(pmhParent)	{

	// TODO
}

CEngineTemplate::~CEngineTemplate()	{

	// TODO
}


int CEngineTemplate::drawFrame() {

	// TODO : Fase de renderizaci�n del Motor.

	// Ejemplo de renderizaci�n simple suponiendo que disponemos de un Conjunto de C�maras

	/*for (int i=0;i<camaras.size();i++) {

		camaras[i]->locateRenderScene();
		camaras[i]->render();

	}

	Write.refresh();*/

	return EXITO;
}

int  CEngineTemplate::onInit() {

	// TODO : Inicializaci�n de recursos no cargados necesarios en 'onIdle'

	return EXITO;

}

//idle. Main loop.
int CEngineTemplate::onIdle()	{

	// TODO : L�gica principal del Motor.

	return EXITO;

}

int CEngineTemplate::onExit() {

	// TODO : Liberaci�n de recursos no necesarios mientras no haya ejecuci�n de 'onIdle'.

	return EXITO;

}

void CEngineTemplate::deselect() {
	FSEngine::deselect();

	// TODO : Siempre se debe llamar previamente al m�todo de la clase base.

}

int CEngineTemplate::loop() {

	// TODO : Siempre se debe llamar posteriormente al m�todo de la clase base.

	return FSEngine::loop();
}

	 
int CEngineTemplate::onMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2) {

	// TODO : Gesti�n de mensajes

	return EXITO;

}

	 
void CEngineTemplate::pendingMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2) {

	// TODO : Gesti�n de mensajes

}