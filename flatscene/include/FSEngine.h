#ifndef __EVENTHANDLER_H__
#define __EVENTHANDLER_H__

#include "FSTime.h"
#include <map>
#include <functional>
#include <memory>

class FSLibrary;

class FSEngine : public std::enable_shared_from_this<FSEngine> {
private:


    bool initialized;

    std::map<Uint8,std::function<void (SDL_Event*)> > eventHandlerRegister2;

    friend class FSLibrary;

protected:

    int priority;

    virtual int onInit();
    
    virtual int onExit();

    virtual int loop();

    
    virtual int onIdle();

    virtual void deselect();



public:
    bool done;

    
    FSEngine();
    
    virtual ~FSEngine();


    bool isInitialized();

    virtual int drawFrame();

    void setEventHandler(Uint8 type,void (eventHandler)(SDL_Event*));

    template <class T> void setEventHandler(Uint8 type,void (T::*eventHandler)(SDL_Event*)) {

        std::function<void (SDL_Event*)> f;

        f = std::bind1st( std::mem_fun(eventHandler) , (T*)this);
        
        std::function<void (SDL_Event*)> ret;

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
    };/**/

    void foo() { printf("probando\n"); };
    template <class T> void nocompila(void (T::*puntero)(void));/*

        boost::function<void ()> f;
    
        f = std::bind1st( mem_fun(puntero), (T*) this);

        f();
    }*/
};

#endif //#ifndef __EVENTHANDLER_H__
