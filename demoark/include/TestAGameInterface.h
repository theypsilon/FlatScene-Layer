#ifndef __TESTAGAMEINTERFACE_H__
#define __TESTAGAMEINTERFACE_H__

#include "FSEngine.h"

#include "FSdefinitions.h"
#include "ScrollCamera.h"
#include "FSMultiverse.h"
 
#include <vector>
#include <map>
#include <list>
#include <functional>

typedef struct{
    Uint8 player;
    Uint8 key;
}KeySDLAlias;

typedef struct
{
    GLuint texture;
    int w, h;
    int w_or, h_or;
    int posx, posy;
} Textura;

typedef std::vector<FSCamera*> CameraCollection;

class CPlayer;
class CEnemy;

class CTestAGameInterface : public FSEngine {
public:
    FSSpriteset* m_pSpriteset;
    FSPoint m_ptCurrent;
    FSPoint m_ptNext;
#ifdef EVENTOS_RAPIDO
    KeySDLAlias PlayerKeyAlias[NUM_SDLKEY];
#endif

    std::vector<CPlayer*> player;
    std::list<CEnemy*> enemy;

    CameraCollection cams;

    FSMultiverse Cosmos;

    std::list< std::function<void()>> endTasks;

public:
    CTestAGameInterface();
    virtual ~CTestAGameInterface();
    virtual int onInit()=0;
    virtual int drawFrame();

    virtual int onIdle();
    virtual int loop();
    virtual void deselect();

    virtual void onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode);
    virtual void onKeyUp(SDLKey sym,SDLMod mod,Uint16 unicode);

#ifndef EVENTOS_RAPIDO
    int getIndexByKey(SDLKey sym,int* p);
#else
    virtual void erasePlayerKeyAlias(int n);
    virtual void updatePlayerKeyAlias(int n, CPlayer* pyer, int key);
#endif

    virtual void changeMap(CActorScrollMap& actor,int direction);
    virtual void deleteMap(FSUniverse* map);
    virtual void killEnemy(FSActor* victim, FSActor* murder, FSUniverse* map);

    //TODO provisional

    void onKeyTestA(SDL_Event*);

};

#endif //#ifndef __TESTEVENTHANDLER_H__
