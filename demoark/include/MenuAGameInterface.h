#ifndef __MENUAGAMEINTERFACE_H__
#define __MENUAGAMEINTERFACE_H__

#include "FSEngine.h"
#include "FSMessageHandler.h"

#include "FSdefinitions.h"
#include "FSPoint.h"
#include <vector>
#include <string>
#include <list>
 
/*
    ==CMenuAGameInterface==
    CEngine componEnt test.
*/
class CMenuAGameInterface : public FSEngine, public FSMessageHandler {
private:
    bool pushed;

    Uint8 opcion;

    int file;
    std::vector<int> IdTexts;
    std::list<std::string> texts;

    FSPoint dest;

    std::shared_ptr<FSEngine> previous;
public:
    
    CMenuAGameInterface(FSMessageHandler * pmhParent=NULL);
    
    virtual ~CMenuAGameInterface();
    
    virtual int onInit();
    
    virtual int onIdle();
    
    virtual int onExit();

    std::shared_ptr<FSEngine> setPrevious(std::shared_ptr<FSEngine>);
    
    void onKeyMenu(SDL_Event*);

    virtual void onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode);
    virtual void onKeyUp(SDLKey sym,SDLMod mod,Uint16 unicode);
};

#endif 
