#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "TestAGameInterface.h"
#include "ActorScrollMap.h"
#include "FSPoint.h"
#include "TileBG.h"
#include <list>
#include "FSparserXML.h" 


class CPlayer : public CActorScrollMap {
public:
    Uint16 KeyAlias[NUM_PJKEY];

// Las siguiente son de caracter más publico todavía.

    void configKey(SDLKey sym,const char* keycode);
    int KeyAliasFor(SDLKey sym);

    CPlayer(CTestAGameInterface& game);
    ~CPlayer();

    void blockFutureActionCandidates();

    void init(std::list<std::string>& activationIds,int v);
    int move();
    FSPoint m_Movxy;

#ifdef MENSAJES_ACCIONES
    int textA0;
    int textA1;
    int textA2;
    int textA3;
#endif
};



#endif
