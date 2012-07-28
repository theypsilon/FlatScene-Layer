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
        virtual void loadUniverse();
        virtual Universe* getUniverse();
        virtual void unloadUniverse();
        virtual bool isOpened();

        int& CX();
        int& CY();
        Actor* Target();
        virtual void setTarget(Actor* newTarget);
        Rectangle* getArea();

        virtual void refresh();
        void render();
        virtual void reubicate(Rectangle* nArea);

        void rotate(Float angle, Float x=0.0, Float y=0.0, Float z=1.0);
        void translate(Float x, Float y, Float z);
        void scale(Float x, Float y, Float z);
        void color(Float red, Float green, Float blue, Float alpha);
        void color(Color* col,Float alpha=1.0);
    
        void locateRenderScene ( Float posx=0, Float posy=0, Float width=0, Float height=0, Float zoom = 1.0);
        void locateRenderScene ( Rectangle* areaSc, Float zoom = 1.0);

        #ifdef MENSAJES_MSGIDS
        void SendMessage(Uint32 MsgID,MSGPARM ParmMsg=NULL);
        #endif
    };

    void resyncUniverse(Camera& cam);

} // flatscene

#endif