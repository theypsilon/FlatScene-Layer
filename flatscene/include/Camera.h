#ifndef ___CAMERA___
#define ___CAMERA___

#include "Actor.h"
 
#include "Types.h"
#include "Canvas.h"
#include "Screen.h"
#include <vector>
#include <string>

namespace FlatScene {

    class Camera {
    protected:
        int x,y;
        Actor* target;
        Universe* uni;
        Rectangle* area;

        bool rendering;

        std::list<std::function<void()>> initRenderList;
        std::list<std::function<void()>> endRenderList;

    public:
    
        Camera(Actor* target,Rectangle* area);
        virtual ~Camera();
        virtual int loadUniverse();
        virtual Universe* getUniverse();
        virtual int unloadUniverse();
        virtual int resyncUniverse();
        virtual bool isOpened();

        int& CX();
        int& CY();
        Actor* Target();
        virtual int setTarget(Actor* newTarget);
        Rectangle* getArea();

        virtual int refresh();
        int render();
        virtual int reubicate(Rectangle* nArea);

        int rotate(Float angle, Float x=0.0, Float y=0.0, Float z=1.0);
        int translate(Float x, Float y, Float z);
        int scale(Float x, Float y, Float z);
        int color(Float red, Float green, Float blue, Float alpha);
        int color(Color* col,Float alpha=1.0);
    
        int locateRenderScene ( Float posx=0, Float posy=0, Float width=0, Float height=0, Float zoom = 1.0);
        int locateRenderScene ( Rectangle* areaSc, Float zoom = 1.0);

        #ifdef MENSAJES_MSGIDS
        int SendMessage(Uint32 MsgID,MSGPARM ParmMsg=NULL);
        #endif
    };

} // flatscene

#endif