#ifndef __SCROLL_CAMERA_H__
#define __SCROLL_CAMERA_H__

#include "FSCamera.h"
#include "ActorScrollMap.h"
 
#include "LayerBackground.h"
#include "FSRectangle.h"
#include "FSCanvas.h"
#include "Map.h"
#include "FSControlMultiverse.h"
#include <vector>
#include <string>

using namespace std;

class CScrollCamera : public FSCamera {
private:
	FSPoint* centro;
	int intraMargenX, intraMargenY;
	FloorCollection Pisos;
	float zoom;
public:
	CScrollCamera(FSActor* target,FSRectangle* area, FSPoint* centro,float margenDesp=0.0,FSMessageHandler * pmhParent=NULL);
	CScrollCamera(FSActor* target,FSRectangle* area,FSMessageHandler * pmhParent=NULL);
	virtual ~CScrollCamera();
	virtual int loadUniverse();
	virtual int unloadUniverse();
	virtual bool focusActor(FSActor* actAct);
	virtual int refresh();
	virtual void setZoom(float newZoom);

#ifdef MENSAJES_MSGIDS
	int SendMessage(Uint32 MsgID,MSGPARM ParmMsg=NULL);
#endif
};

#endif