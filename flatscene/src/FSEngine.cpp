// Engine.cpp: implementation of the CEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "FSEngine.h"
#include "FSLibrary.h"
#include "FSMultiverse.h"
#include "FSWriter.h"



FSEngine::FSEngine(FSMessageHandler * pmhParent) : FSMessageHandler(pmhParent)
{
	initialized = false;
	priority = 100;
	done = true;
}


FSEngine::~FSEngine()
{
		readMessages();
}
bool FSEngine::isInitialized() {
	return initialized;
}

int FSEngine::onInit() {

	initialized = true;

	return EXITO;
}

void FSEngine::setEventHandler(Uint8 type,void (eventHandler)(SDL_Event*)) {

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

template <class T> void FSEngine::nocompila(void (T::*puntero)(void)) {

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
int FSEngine::loop() {

	if (!isInitialized()) {
		FSLibrary::Error("Engine not inicialized");
		return FRACASO;
	}

	FSLibrary::setActualEngine(this);
	
	SDL_Event event;

	while (SDL_PollEvent(&event)==1);
		
	while (FSLibrary::getActualEngine() == this) {
			
		if(
#ifdef DEBUGTEST
			!FSLibrary::inDebug() && 
#endif
			SDL_PollEvent(&event)==1) {
				
			if(event.type==SDL_QUIT) {
				FSLibrary::getLibrary().SendMessage(FSLibrary::MSGID_Exit);
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

void FSEngine::deselect() {
	SDL_Event event;
	while (SDL_PollEvent(&event)==1) { }
	FSLibrary::setActualEngine(NULL);
}

int FSEngine::drawFrame() {

	if (!isInitialized()) {
		FSLibrary::Error("Motor not inicialized");
		return FRACASO;
	}

	return EXITO;
}


int FSEngine::onIdle()
{
	return EXITO;
}


int FSEngine::onExit()
{
	int ret = EXITO;

	//TODO comprobar valor de vuelta de las siguientes 2 funciones.

	Cosmos.clear();
	Write.erase();

	initialized = false;

	return ret;
}
