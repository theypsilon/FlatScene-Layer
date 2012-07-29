#ifndef __EVENTHANDLER_H__
#define __EVENTHANDLER_H__

#include "Includes.h"
#include <map>
#include <functional>
#include <memory>

namespace FlatScene {

    class Library;

    class Engine : public std::enable_shared_from_this<Engine> {
        friend class Library;
        bool initialized;
    protected:
        int priority;

        virtual void onEvent(const SDL_Event& event);
        virtual void onInit();
        virtual void onExit();
        virtual void loop();
        virtual void onIdle();
        virtual void deselect();

    public:
        bool done;

        Engine();
        virtual ~Engine();

        bool isInitialized();

        virtual void drawFrame();
    };

} // flatscene

#endif //#ifndef __EVENTHANDLER_H__
