#ifndef CINEMA_ENGINE_IMPL__
#define CINEMA_ENGINE_IMPL__

#include "Engine.h"
#include "Exception.h"

namespace Cinema {

    inline CompleteEngine::CompleteEngine() {
        initialized = false;
        priority = 100;
        done = true;
    }

    inline CompleteEngine::~CompleteEngine() {}

    inline bool CompleteEngine::isInitialized() {
        return initialized;
    }

    inline void CompleteEngine::onInit() {
        initialized = true;
    }

    inline void CompleteEngine::onEvent(const FlatScene::Event& event) {
        if(event.getType() == FlatScene::EventType::QUIT) {
            done = false;
        }
    }

    //bucle principal
    inline void CompleteEngine::loop() {

        if (!isInitialized()) throw FlatScene::Exception("CompleteEngine not inicialized", __LINE__);

        auto self = this;

        FlatScene::FreeAllEvents();

        while (done) {

            for (const auto& event : FlatScene::PollEvents()) {
                onEvent(event);
            }

            onIdle();
            drawFrame();
        }
    }

    inline void CompleteEngine::deselect() {
        FlatScene::FreeAllEvents();
    }

    inline void CompleteEngine::drawFrame() {
        if (!isInitialized()) throw FlatScene::Exception("Motor not inicialized", __LINE__);
    }


    inline void CompleteEngine::onIdle() {}


    inline void CompleteEngine::onExit() {
        initialized = false;
    }
} // Cinema

#endif // CINEMA_ENGINE_IMPL__