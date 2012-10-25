#ifndef CINEMA_ENGINE__
#define CINEMA_ENGINE__

#include "Event.h"

#include <map>
#include <functional>
#include <memory>

namespace Cinema {
    class Library;

    class CompleteEngine : public FlatScene::Engine {
        friend class Library;
        bool initialized;
    protected:
        int priority;

        virtual void onEvent(const FlatScene::Event& event);
        virtual void onInit();
        virtual void onExit();
        virtual void loop();
        virtual void onIdle();
        virtual void deselect();

    public:
        bool done;

        CompleteEngine();
        virtual ~CompleteEngine();

        bool isInitialized();

        virtual void drawFrame();
    };
} // Cinema

#include "Engine-impl.h"

#endif // CINEMA_ENGINE__