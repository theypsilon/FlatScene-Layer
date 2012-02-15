#ifndef __MENUAGAMEINTERFACE_H__
#define __MENUAGAMEINTERFACE_H__

#include "FSEngine.h"
#include "FSdefinitions.h"
#include "FSPoint.h"
#include <vector>
#include <string>
 
/*
    ==CMenuAGameInterface==
    CEngine componEnt test.
*/
class CMenuAGameInterface : public FSEngine  
{
private:
    bool pushed;

    Uint8 opcion;

    int file;
    std::vector<int> IdTexts;
    std::list<std::string> texts;

    FSPoint dest;

    FSEngine* previous;
public:
    //constructor
    CMenuAGameInterface(FSMessageHandler * pmhParent=NULL);
    //destructor
    virtual ~CMenuAGameInterface();
    //initialization
    virtual int onInit();
    //idle behavior
    virtual int onIdle();
    //cleanup
    virtual int onExit();

    FSEngine* setPrevious(FSEngine*);
    
    void onKeyMenu(SDL_Event*);

    virtual void onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode);
    virtual void onKeyUp(SDLKey sym,SDLMod mod,Uint16 unicode);
};

#endif 
