#ifndef __MAINCANVAS_H__
#define __MAINCANVAS_H__

#include "Canvas.h"
#include "Color.h"
#include <list>

#include "Singleton.h"
#include "Noncopyable.h"

namespace FlatScene {

    #define RENDER_TEXTURE_STANDARD 0x00000001

    enum TypeRendeProjection {
        TRP_ORTHO,
        TRP_PERSPECTIVE
    };

    struct ScreenImpl;

    class Screen : private Util::NonCpmvable, public Util::Singleton<Screen> {
        friend class Util::Singleton<Screen>;
    public:
        //constructor
        int start ( int width , int height , int bpp , bool fullscreen, bool doublebuff=true ) ;
        int start ( int width , int height , int bpp , Float scalex, Float scaley,bool fullscreen, bool doublebuff=true ) ;

        int render();

        int clear ();

        int rotate(Float angle, Float x=0.0, Float y=0.0, Float z=1.0);
        int translate(Float x, Float y, Float z);
        int scale(Float x, Float y, Float z);
        int color(Float red, Float green, Float blue, Float alpha);
        int color(Color* col,Float alpha=1.0);
        int projectionMode(TypeRendeProjection trp, Float zMax=400.0);

        int pushMatrix();
        int popMatrix();

        int locateRenderScene ( Float posx=0.0, Float posy=0.0, Float width=0.0, Float height=0.0, Float zoom = 1.0) ;

        unsigned char getBpp();

        int changeScreen( int width , int height , int bpp ,  Float scalex=1.0, Float scaley=1.0, bool fullscreen = false) ;

        int ToggleFullscreen ();

        int setDoublebuffer (bool doublebuff);
        //destructor
        int quit();

        int getWidth();

        int getHeight();

        bool isFullscreen();

        Float getR(),getG(),getB(),getA();

    private:
        ScreenImpl* _impl;

        Screen();
        virtual ~Screen();

        friend class Camera;

        friend class Spriteset;
        friend class Sprite;
        friend class Canvas;

        friend struct SRenderLocation;
    };
    #ifdef GLOBAL_SINGLETON_REFERENCES
    extern Screen& FSDraw;
    #endif

} // flatscene

#endif 
