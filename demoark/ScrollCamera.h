#ifndef __SCROLL_CAMERA_H__
#define __SCROLL_CAMERA_H__

#include "FSCamera.h"
#include "ActorScrollMap.h"
 
#include "LayerBackground.h"
#include "FSRectangle.h"
#include "FSImage.h"
#include "Map.h"
#include "FSControlMultiverse.h"
#include <vector>
#include <string>

using namespace std;

class CScrollCamera : public CCamera {
private:
	CPoint* centro;
	int intraMargenX, intraMargenY;
	FloorCollection Pisos;
	float zoom;
public:
	CScrollCamera(CActor* target,CRectangle* area, CPoint* centro,float margenDesp=0.0,CMessageHandler * pmhParent=NULL);
	CScrollCamera(CActor* target,CRectangle* area,CMessageHandler * pmhParent=NULL);
	virtual ~CScrollCamera();
	virtual int loadUniverse();
	virtual int unloadUniverse();
	virtual bool focusActor(CActor* actAct);
	virtual int refresh();
	virtual void setZoom(float newZoom);

#ifdef MENSAJES_MSGIDS
	int SendMessage(Uint32 MsgID,MSGPARM ParmMsg=NULL);
#endif
};

#endif