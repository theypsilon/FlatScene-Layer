#ifndef ___CAMERA___
#define ___CAMERA___

#include "MessageHandler.h"
#include "Actor.h"
 
#include "Rectangle.h"
#include "Image.h"
#include "ControlMultiverse.h"
#include "Screen.h"
#include <vector>
#include <string>

using namespace std;

class CCamera : public CMessageHandler {
protected:
	int x,y;
	CActor* target;
	CUniverse* uni;
	CRectangle* area;

	bool rendering;

	list<SToRender*> initRenderList;
	list<SToRender*> endRenderList;

	static map<TypeResource,void (*)(void*)> procRenders;

	static void initProcRenders();

	static void procRendRotation(void* pointer);
	static void procRendTranslation(void* pointer);
	static void procRendLocation(void* pointer);
	static void procRendScalation(void* pointer);
	static void procRendColor(void* pointer);
	static void procRendPush(void* pointer);
	static void procRendPop(void* pointer);
public:
	
	CCamera(CActor* target,CRectangle* area,CMessageHandler* pmhParent=NULL);
	virtual ~CCamera();
	virtual int loadUniverse();
	virtual CUniverse* getUniverse();
	virtual int unloadUniverse();
	virtual int resyncUniverse();
	virtual bool isOpened();

	int& CX();
	int& CY();
	CActor* Target();
	virtual int setTarget(CActor* newTarget);
	CRectangle* getArea();

	virtual int refresh();
	int render();
	virtual int reubicate(CRectangle* nArea);

	int rotate(float angle, float x=0.0, float y=0.0, float z=1.0);
	int translate(float x, float y, float z);
	int scale(float x, float y, float z);
	int color(float red, float green, float blue, float alpha);
	int color(CColor* col,float alpha=1.0);
	
	int locateRenderScene ( float posx=0, float posy=0, float width=0, float height=0, float zoom = 1.0);
	int locateRenderScene ( CRectangle* areaSc, float zoom = 1.0);

#ifdef MENSAJES_MSGIDS
	int SendMessage(Uint32 MsgID,MSGPARM ParmMsg=NULL);
#endif
};

#endif