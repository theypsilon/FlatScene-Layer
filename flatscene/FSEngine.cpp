// Engine.cpp: implementation of the CEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "FSEngine.h"
#include "FSLibrary.h"
#include "FSControlMultiverse.h"
#include "FSControlOutputText.h"



CEngine::CEngine(CMessageHandler * pmhParent) : CMessageHandler(pmhParent)
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

void CEngine::setEventHandler(Uint8 type,void (eventHandler)(SDL_Event*)) {

	if (this->eventHandlerRegister2.find(type) == this->eventHandlerRegister2.end())
		;//ret = NULL;
	else {
		//ret = this->eventHandlerRegister2[type];

		if (eventHandler == NULL) {
			this->eventHandlerRegister2.erase(eventHandlerRegister2.find(type));
			//return ret;
		}
	}

	this->eventHandlerRegister2[type] = eventHandler;

}

template <class T> void CEngine::nocompila(void (T::*puntero)(void)) {

	boost::function<void ()> f;
	
	f = std::bind1st( mem_fun(puntero), (T*) this);

	f();
}
/*
template <class T>
void CEngine::setEventHandler(Uint8 type,void (T::*eventHandler)(SDL_Event*)) {
	boost::function<void (SDL_Event*)> f;

	f = std::bind1st( mem_fun(eventHandler) , (T*)this);
		
	boost::function<void (SDL_Event*)> ret;

	printf("culo de mono\n");

	if (this->eventHandlerRegister2.find(type) == this->eventHandlerRegister2.end())
		;//ret = NULL;
	else {
		ret = this->eventHandlerRegister2[type];

		if (eventHandler == NULL) {
			this->eventHandlerRegister2.erase(eventHandlerRegister2.find(type));
			return;
		}
	}

	this->eventHandlerRegister2[type] = f;
}/*

const void* CEngine::setEventHandler(Uint8 type,void (eventHandler)(SDL_Event*)) {

	const void* ret;

	if (this->eventHandlerRegister.find(type) == this->eventHandlerRegister.end())
		ret = NULL;
	else {
		ret = this->eventHandlerRegister[type];

		if (eventHandler == NULL) {
			this->eventHandlerRegister.erase(eventHandlerRegister.find(type));
			return ret;
		}
	}

	this->eventHandlerRegister[type] = (const void*) eventHandler;

	return ret;
}*/



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
			if (eventHandlerRegister2.find(event.type)!=eventHandlerRegister2.end()) 
				eventHandlerRegister2[event.type](&event);
				
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
