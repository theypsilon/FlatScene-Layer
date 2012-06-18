// Engine.cpp: implementation of the CEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "Engine.h"
#include "LibraryImpl.h"
#include "Multiverse.h"
#include "Writer.h"
#include "Exception.h"

namespace FlatScene {

Engine::Engine() {
    initialized = false;
    priority = 100;
    done = true;
}


Engine::~Engine() {}

bool Engine::isInitialized() {
    return initialized;
}

void Engine::onInit() {
    initialized = true;
}

void Engine::onEvent(const SDL_Event& event) {
    if(event.type==SDL_QUIT) {
        Library::I().exit();
    }
}

//bucle principal
void Engine::loop() {

    if (!isInitialized()) {
        throw Exception("Engine not inicialized", __LINE__);
    }

    auto self = this;

    Library::I()._impl->setActualEngine(self);
    
    SDL_Event event;

    while (SDL_PollEvent(&event)==1);

    while (Library::I().getActualEngine() == self) {

        if(SDL_PollEvent(&event)==1) {
            onEvent(event);
        } else {
            onIdle();
            drawFrame();
        }
    }
}

void Engine::deselect() {
    SDL_Event event;
    while (SDL_PollEvent(&event)==1) { }
    Library::I()._impl->setActualEngine(NULL);
}

void Engine::drawFrame() {
    if (!isInitialized()) {
        throw Exception("Motor not inicialized", __LINE__);
    }
}


void Engine::onIdle() {}


void Engine::onExit() {
    Writer::I().erase();
    initialized = false;
}

} // flatscene
