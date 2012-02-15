// Engine.cpp: implementation of the CEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "FSEngine.h"
#include "FSLibraryImpl.h"
#include "FSMultiverse.h"
#include "FSWriter.h"



FSEngine::FSEngine() {
    initialized = false;
    priority = 100;
    done = true;
}


FSEngine::~FSEngine() {}

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

    std::function<void ()> f;
    
    f = std::bind1st( std::mem_fun(puntero), (T*) this);

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
}

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
        FSLibrary::I().Error("Engine not inicialized");
        return FRACASO;
    }

    auto self = shared_from_this();

    FSLibrary::I()._impl->setActualEngine(self);
    
    SDL_Event event;

    while (SDL_PollEvent(&event)==1);

    while (FSLibrary::I().getActualEngine() == self) {

        if(
#ifdef DEBUGTEST
            !FSLibrary::I().inDebug() &&
#endif
            SDL_PollEvent(&event)==1) {

            if(event.type==SDL_QUIT) {
                FSLibrary::I().getLibrary().SendMessage(FSLibrary::I().MSGID_Exit);
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
    FSLibrary::I()._impl->setActualEngine(NULL);
}

int FSEngine::drawFrame() {

    if (!isInitialized()) {
        FSLibrary::I().Error("Motor not inicialized");
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

    FSMultiverse::I().clear();
    FSWriter::I().erase();

    initialized = false;

    return ret;
}
