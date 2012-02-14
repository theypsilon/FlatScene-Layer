#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "FSMessageHandler.h"
#include "ActorScrollMap.h"
#include "FSPoint.h"
#include "TileBG.h"
#include <list>
#include "FSparserXML.h" 
using namespace std;

class CPlayer : public CActorScrollMap {
public:
    Uint16 KeyAlias[NUM_PJKEY];

// Las siguiente son de caracter más publico todavía.

    void configKey(SDLKey sym,const char* keycode);
    int KeyAliasFor(SDLKey sym);

    CPlayer(FSMessageHandler * pmhParent=NULL);
    ~CPlayer();

    void blockFutureActionCandidates();

    void init(list<string>& activationIds,int v);
    int move();
    FSPoint m_Movxy;

    int onMessage(Uint32 MsgID,MSGPARM Parm1=NULL,MSGPARM Parm2=NULL); 

#ifdef MENSAJES_MSGIDS
    int SendMessage(Uint32 MsgID,MSGPARM Parm1=NULL,MSGPARM Parm2=NULL);
#endif
#ifdef MENSAJES_ACCIONES
    int textA0;
    int textA1;
    int textA2;
    int textA3;
#endif
};



#endif