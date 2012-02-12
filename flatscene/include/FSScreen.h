#ifndef __MAINCANVAS_H__
#define __MAINCANVAS_H__

#include "FSCanvas.h"
#include "FSImages.h"
#include "FSColor.h"
#include <list>

#include "FSSingleton.h"
#include "FSNoncopyable.h"

#define RENDER_TEXTURE_STANDARD 0x00000001

typedef list<SpritesetInfo> GraphicResources;

enum TypeRendeProjection {
	TRP_ORTHO,
	TRP_PERSPECTIVE
};

struct SRender {
	virtual void operator()()=0;
};

struct SToRender {
	TypeResource type;
	SRender* pointer;
};

struct SRenderLocation : SRender { // LOCATE
	float posx;
	float posy;
	float width;
	float height;

	void operator()();
};

struct SRenderTranslation : SRender { // TRANSLATE && SCALATION
	float x;
	float y;
	float z;

	void operator()();
};

struct SRenderScalation : SRender { // TRANSLATE && SCALATION
	float x;
	float y;
	float z;

	void operator()();
};

struct SRenderRotation : SRender { // ROTATION
	float angle;
	float x;
	float y;
	float z;

	void operator()();
};

struct SRenderColor : SRender { // ROTATION
	float red;
	float green;
	float blue;
	float alpha;

	void operator()();
};

struct SRenderCanvasFloat : SRender {
	SCanvas canvas;
	FSFloatPoint ptDst;
	Uint8 flags;

	SRenderCanvasFloat(SCanvas canvas, FSFloatPoint ptDst, Uint8 flags)
	: canvas(canvas), ptDst(ptDst), flags(flags) {
	}

	void operator()();
};

struct SRenderCanvasInt : SRender {
	SCanvas canvas;
	FSPoint ptDst;
	Uint8 flags;

	SRenderCanvasInt(SCanvas canvas, FSPoint ptDst, Uint8 flags)
	: canvas(canvas), ptDst(ptDst), flags(flags) {
	}

	void operator()();
};


struct SRenderPushMatrix : SRender { // ROTATION
	void operator()();
};

struct SRenderPopMatrix : SRender { // ROTATION
	void operator()();
};


class FSScreen : private FSNoncopyable, public FSSingleton<FSScreen> {
    friend class FSSingleton<FSScreen>;
public:
	//constructor
	int start ( int width , int height , int bpp , bool fullscreen, bool doublebuff=true ) ;
	int start ( int width , int height , int bpp , float scalex, float scaley,bool fullscreen, bool doublebuff=true ) ;

	int render();

	int clear ( ) ;

	int rotate(float angle, float x=0.0, float y=0.0, float z=1.0);
	int translate(float x, float y, float z);
	int scale(float x, float y, float z);
	int color(float red, float green, float blue, float alpha);
	int color(FSColor* col,float alpha=1.0);
	int projectionMode(TypeRendeProjection trp, float zMax=400.0);

	int pushMatrix();
	int popMatrix();

	int locateRenderScene ( float posx=0.0, float posy=0.0, float width=0.0, float height=0.0, float zoom = 1.0) ;

	Uint8 getBpp();

	int changeScreen( int width , int height , int bpp ,  float scalex=1.0, float scaley=1.0, bool fullscreen = false) ;

	int ToggleFullscreen ();

	int setDoublebuffer (bool doublebuff);
	//destructor
	int quit();

	int getWidth();

	int getHeight();

	bool isFullscreen();

	float getR(),getG(),getB(),getA();

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

	friend class SRenderLocation;
};
#ifdef GLOBAL_SINGLETON_REFERENCES
extern FSScreen& FSDraw;
#endif


#endif 
