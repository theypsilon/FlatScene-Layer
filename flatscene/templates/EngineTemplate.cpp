#include "EngineTemplate.h"

CEngineTemplate::CEngineTemplate(FSMessageHandler* pmhParent) : FSEngine(pmhParent)	{

	// TODO
}

CEngineTemplate::~CEngineTemplate()	{

	// TODO
}


int CEngineTemplate::drawFrame() {

	// TODO : Fase de renderización del Motor.

	// Ejemplo de renderización simple suponiendo que disponemos de un Conjunto de Cámaras

	/*for (int i=0;i<camaras.size();i++) {

		camaras[i]->locateRenderScene();
		camaras[i]->render();

	}

	Write.refresh();*/

	return EXITO;
}

int  CEngineTemplate::onInit() {

	// TODO : Inicialización de recursos no cargados necesarios en 'onIdle'

	return EXITO;

}

//idle. Main loop.
int CEngineTemplate::onIdle()	{

	// TODO : Lógica principal del Motor.

	return EXITO;

}

int CEngineTemplate::onExit() {

	// TODO : Liberación de recursos no necesarios mientras no haya ejecución de 'onIdle'.

	return EXITO;

}

void CEngineTemplate::deselect() {
	FSEngine::deselect();

	// TODO : Siempre se debe llamar previamente al método de la clase base.

}

int CEngineTemplate::loop() {

	// TODO : Siempre se debe llamar posteriormente al método de la clase base.

	return FSEngine::loop();
}

	 
int CEngineTemplate::onMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2) {

	// TODO : Gestión de mensajes

	return EXITO;

}

	 
void CEngineTemplate::pendingMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2) {

	// TODO : Gestión de mensajes

}