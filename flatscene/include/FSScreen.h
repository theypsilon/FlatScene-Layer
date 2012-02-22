#ifndef __MAINCANVAS_H__
#define __MAINCANVAS_H__

#include "FSCanvas.h"
#include "FSImages.h"
#include "FSColor.h"
#include <list>

#include "FSSingleton.h"
#include "FSNoncopyable.h"

#define RENDER_TEXTURE_STANDARD 0x00000001

typedef std::list<SpritesetInfo> GraphicResources;

enum TypeRendeProjection {
    TRP_ORTHO,
    TRP_PERSPECTIVE
};

class FSScreen : private FSNoncopyable, public FSSingleton<FSScreen> {
    friend class FSSingleton<FSScreen>;
public:
    //constructor
    int start ( int width , int height , int bpp , bool fullscreen, bool doublebuff=true ) ;
    int start ( int width , int height , int bpp , Float scalex, Float scaley,bool fullscreen, bool doublebuff=true ) ;

    int render();

    int clear ( ) ;

    int rotate(Float angle, Float x=0.0, Float y=0.0, Float z=1.0);
    int translate(Float x, Float y, Float z);
    int scale(Float x, Float y, Float z);
    int color(Float red, Float green, Float blue, Float alpha);
    int color(FSColor* col,Float alpha=1.0);
    int projectionMode(TypeRendeProjection trp, Float zMax=400.0);

    int pushMatrix();
    int popMatrix();

    int locateRenderScene ( Float posx=0.0, Float posy=0.0, Float width=0.0, Float height=0.0, Float zoom = 1.0) ;

    Uint8 getBpp();

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
    struct ScreenImpl;
    ScreenImpl* _impl;

    FSScreen();
    virtual ~FSScreen();

    friend class FSCamera;

    friend class FSSpriteset;
    friend class FSSprite;
    friend class FSCanvas;

    friend class FSImages;
    friend class FSWriter;
    friend class FSTextBox;
};
#ifdef GLOBAL_SINGLETON_REFERENCES
extern FSScreen& FSDraw;
#endif


#endif 
