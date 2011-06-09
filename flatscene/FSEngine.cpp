// Engine.cpp: implementation of the CEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "FSEngine.h"
#include "FSLibrary.h"
#include "FSControlMultiverse.h"
#include "FSControlOutputText.h"



CEngine::CEngine(CMessageHandler* pmhParent) : CMessageHandler(pmhParent)
{
	initialized = false;
	priority = 100;
	done = true;
}


CEngine::~CEngine()
{
		readMessages();
}
bool CEngine::isInitialized() {
	return initialized;
}

int CEngine::onInit() {

	initialized = true;

	return EXITO;
}

const void* CEngine::setEventHandler(Uint8 type,void (eventHandler)(SDL_Event*)) {

	const void* ret;

	if (this->eventHandlerRegister.find(type) == this->eventHandlerRegister.end())
		ret = NULL;
	else {
		ret = this->eventHandlerRegister[type];

		if (eventHandler == NULL) {
			this->eventHandlerRegister.erase(eventHandlerRegister.find(type))
			return ret;
		}
	}

	this->eventHandlerRegister[type] = (const void*) eventHandler;

	return ret;
}

//bucle principal
int CEngine::loop() {

	if (!CLibrary::getLibrary()) {
		CLibrary::Error("Library not inicialized");
		return FRACASO;
	}

	if (!isInitialized()) {
		CLibrary::Error("Engine not inicialized");
		return FRACASO;
	}

	CLibrary::setActualEngine(this);
	
	SDL_Event event;

	while (SDL_PollEvent(&event)==1);
		
	while (CLibrary::getActualEngine() == this) {
			
		if(
#ifdef DEBUGTEST
			!CLibrary::inDebug() && 
#endif
			SDL_PollEvent(&event)==1) {
				
			if(event.type==SDL_QUIT) {
				CLibrary::getLibrary()->SendMessage(CLibrary::MSGID_Exit);
				break;
			}

			//si hay manejador de eventos registrado para este evento, lo ejecuta
			if (eventHandlerRegister.find(event.type)!=eventHandlerRegister.end()) 
				((void (*)(SDL_Event*)) eventHandlerRegister[event.type])(&event);
				
		} else {
				
			onIdle();

			drawFrame();

			Chrono.nextFrame();	
		}
	}

	return EXITO;

}

void CEngine::deselect() {
	SDL_Event event;
	while (SDL_PollEvent(&event)==1) { }
	CLibrary::setActualEngine(NULL);
}

int CEngine::drawFrame() {

	if (!isInitialized()) {
		CLibrary::Error("Motor not inicialized");
		return FRACASO;
	}

	return EXITO;
}


int CEngine::onIdle()
{
	return EXITO;
}


int CEngine::onExit()
{
	int ret = EXITO;

	//TODO comprobar valor de vuelta de las siguientes 2 funciones.

	CMultiverse.clear();
	Write.erase();

	initialized = false;

	return ret;
}
