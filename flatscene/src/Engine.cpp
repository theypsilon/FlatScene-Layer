// Engine.cpp: implementation of the CEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "Engine.h"
#include "LibraryImpl.h"
#include "Exception.h"
#include "EventBridge.h"
#include "EventAdapter.h"

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

void Engine::onEvent(const Event& event) {
    if(event.getType() == EventType::QUIT) {
        Library::I().exit();
    }
}

//bucle principal
void Engine::loop() {

    if (!isInitialized()) throw Exception("Engine not inicialized", __LINE__);

    auto self = this;

    Library::I()._impl->setActualEngine(self);

    FreeAllEvents();

    while (Library::I().getActualEngine() == self) {

        for (const auto& event : PollEvents()) {
            onEvent(event);
        }

        onIdle();
        drawFrame();
    }
}

void Engine::deselect() {
    FreeAllEvents();
    Library::I()._impl->setActualEngine(nullptr);
}

void Engine::drawFrame() {
    if (!isInitialized()) throw Exception("Motor not inicialized", __LINE__);
}


void Engine::onIdle() {}


void Engine::onExit() {
    initialized = false;
}

} // flatscene
