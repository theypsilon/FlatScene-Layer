// Engine.cpp: implementation of the CEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "FSEngine.h"
#include "FSLibraryImpl.h"
#include "FSMultiverse.h"
#include "FSWriter.h"
#include "FSException.h"

namespace flatscene {

FSEngine::FSEngine() {
    initialized = false;
    priority = 100;
    done = true;
}


FSEngine::~FSEngine() {}

bool FSEngine::isInitialized() {
    return initialized;
}

void FSEngine::onInit() {
    initialized = true;
}

void FSEngine::onEvent(const SDL_Event& event) {
    if(event.type==SDL_QUIT) {
        FSLibrary::I().exit();
    }
}

//bucle principal
void FSEngine::loop() {

    if (!isInitialized()) {
        throw FSException("Engine not inicialized", __LINE__);
    }

    auto self = this;

    FSLibrary::I()._impl->setActualEngine(self);
    
    SDL_Event event;

    while (SDL_PollEvent(&event)==1);

    while (FSLibrary::I().getActualEngine() == self) {

        if(SDL_PollEvent(&event)==1) {
            onEvent(event);
        } else {
            onIdle();
            drawFrame();
        }
    }
}

void FSEngine::deselect() {
    SDL_Event event;
    while (SDL_PollEvent(&event)==1) { }
    FSLibrary::I()._impl->setActualEngine(NULL);
}

void FSEngine::drawFrame() {
    if (!isInitialized()) {
        throw FSException("Motor not inicialized", __LINE__);
    }
}


void FSEngine::onIdle() {}


void FSEngine::onExit() {
    FSWriter::I().erase();
    initialized = false;
}

} // flatscene
