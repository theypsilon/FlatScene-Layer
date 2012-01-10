#ifndef __TESTAGAMEINTERFACE_H__
#define __TESTAGAMEINTERFACE_H__

#include "FSEngine.h"
#include "FSdefinitions.h"
#include "FSScreen.h"
#include "ActionMove.h"
#include "ActionFight.h"
#include "Player.h"
#include "Enemy.h"
#include "ScrollCamera.h"
#include "FSControlMultiverse.h"
#include "FSImages.h"
 
#include "TileBG.h"
#include <vector>
#include <map>
#include <list>
using namespace std;

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

typedef vector<FSCamera*> CameraCollection;

/*
	==CTestAGameInterface==
	CEngine componEnt test.
*/
class CTestAGameInterface : public FSEngine  
{
public:
	//sprite set
	FSSpriteset* m_pSpriteset;
	//current position
	FSPoint m_ptCurrent;
	//new position
	FSPoint m_ptNext;
#ifdef EVENTOS_RAPIDO
	KeySDLAlias PlayerKeyAlias[NUM_SDLKEY];
#endif

	vector<CPlayer*> player;
	list<CEnemy*> enemy;

	CameraCollection cams;

public:
	//constructor
	CTestAGameInterface(FSMessageHandler * pmhParent=NULL);
	//destructor
	virtual ~CTestAGameInterface();
	//initialization
	virtual int onInit()=0;
	//idle behavior
	virtual int drawFrame();

	virtual int onIdle();
	//activadores, desactivadores
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

	virtual void pendingMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2);

#ifdef MENSAJES_MSGIDS
	virtual int SendMessage(Uint32 MsgID,MSGPARM Parm1=NULL,MSGPARM Parm2=NULL);
#endif

	//message ids

	static Uint32 MSGID_ChangeMap;
	static Uint32 MSGID_DeleteMap;
	static Uint32 MSGID_KillEnemy;

	//TODO provisional

	void onKeyTestA(SDL_Event*);

};

#endif //#ifndef __TESTEVENTHANDLER_H__
