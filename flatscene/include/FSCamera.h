#ifndef ___CAMERA___
#define ___CAMERA___

#include "FSMessageHandler.h"
#include "FSActor.h"
 
#include "FSTypes.h"
#include "FSCanvas.h"
#include "FSMultiverse.h"
#include "FSScreen.h"
#include <vector>
#include <string>

class FSCamera {
protected:
    int x,y;
    FSActor* target;
    FSUniverse* uni;
    FSRectangle* area;

    bool rendering;

    std::list<std::function<void()>> initRenderList;
    std::list<std::function<void()>> endRenderList;

public:
    
    FSCamera(FSActor* target,FSRectangle* area);
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

    int rotate(Float angle, Float x=0.0, Float y=0.0, Float z=1.0);
    int translate(Float x, Float y, Float z);
    int scale(Float x, Float y, Float z);
    int color(Float red, Float green, Float blue, Float alpha);
    int color(FSColor* col,Float alpha=1.0);
    
    int locateRenderScene ( Float posx=0, Float posy=0, Float width=0, Float height=0, Float zoom = 1.0);
    int locateRenderScene ( FSRectangle* areaSc, Float zoom = 1.0);

#ifdef MENSAJES_MSGIDS
    int SendMessage(Uint32 MsgID,MSGPARM ParmMsg=NULL);
#endif
};

#endif