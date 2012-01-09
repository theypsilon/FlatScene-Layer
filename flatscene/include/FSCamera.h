#ifndef ___CAMERA___
#define ___CAMERA___

#include "FSMessageHandler.h"
#include "FSActor.h"
 
#include "FSRectangle.h"
#include "FSCanvas.h"
#include "FSControlMultiverse.h"
#include "FSScreen.h"
#include <vector>
#include <string>

using namespace std;

class FSCamera : public FSMessageHandler {
protected:
	int x,y;
	FSActor* target;
	FSUniverse* uni;
	FSRectangle* area;

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
	
	FSCamera(FSActor* target,FSRectangle* area,FSMessageHandler * pmhParent=NULL);
	virtual ~FSCamera();
	virtual int loadUniverse();
	virtual FSUniverse* getUniverse();
	virtual int unloadUniverse();
	virtual int resyncUniverse();
	virtual bool isOpened();

	int& CX();
	int& CY();
	FSActor* Target();
	virtual int setTarget(FSActor* newTarget);
	FSRectangle* getArea();

	virtual int refresh();
	int render();
	virtual int reubicate(FSRectangle* nArea);

	int rotate(float angle, float x=0.0, float y=0.0, float z=1.0);
	int translate(float x, float y, float z);
	int scale(float x, float y, float z);
	int color(float red, float green, float blue, float alpha);
	int color(FSColor* col,float alpha=1.0);
	
	int locateRenderScene ( float posx=0, float posy=0, float width=0, float height=0, float zoom = 1.0);
	int locateRenderScene ( FSRectangle* areaSc, float zoom = 1.0);

#ifdef MENSAJES_MSGIDS
	int SendMessage(Uint32 MsgID,MSGPARM ParmMsg=NULL);
#endif
};

#endif