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

struct SToRender {
	TypeResource type;
	void* pointer;
};

struct SRenderCanvas {
	SCanvas canvas;
	FSPoint ptDst;
	Uint8 flags;

	SRenderCanvas(SCanvas canvas, FSPoint ptDst, Uint8 flags) 
	: canvas(canvas), ptDst(ptDst), flags(flags) {
	}
};

struct SRenderFloatCanvas {
	SCanvas canvas;
	FSFloatPoint ptDst;
	Uint8 flags;

	SRenderFloatCanvas(SCanvas canvas,FSFloatPoint ptDst, Uint8 flags) 
	: canvas(canvas), ptDst(ptDst), flags(flags) {
	}
};

struct SRenderLocation { // LOCATE
	float posx;
	float posy; 
	float width; 
	float height; 
	float zoom;
};

struct SRenderTranscalation { // TRANSLATE && SCALATION
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

struct SRenderRotation { // ROTATION
	GLfloat angle;
	GLfloat x; 
	GLfloat y; 
	GLfloat z; 
};

struct SRenderColor { // ROTATION
	GLfloat red;
	GLfloat green; 
	GLfloat blue; 
	GLfloat alpha; 
};


class FSScreen : private FSNoncopyable, public FSSingleton<FSScreen> {
    friend class FSSingleton<FSScreen>;
private:
    struct ScreenImpl;
    ScreenImpl* _impl;

    FSScreen();
    virtual ~FSScreen();

	bool rendering;
	TypeRendeProjection trp;

	float m_maxZ;

	bool m_FullScreen,m_Doublebuff;

	int m_Width,m_Height,m_Bpp;

	list<SToRender*> graphicMaterial;

	list<FSSprite*> spriteToDelete;
	list<FSSpriteset*> spritesetToDelete;
	list<FSCanvas*> imageToDelete;

	void deleteResources();

	void saveResources(GraphicResources& info);
	void reloadResources(GraphicResources& info);


	float red,green,blue,alpha;

	friend class FSCamera;

	friend class FSSpriteset;
	friend class FSSprite;
	friend class FSCanvas;

	friend class FSImages;
	friend class FSWriter;
	friend class FSTextBox;

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

	int getWidth() { return m_Width; };

	int getHeight() { return m_Height; };

	bool isFullscreen() { return m_FullScreen; };

	float getR(),getG(),getB(),getA();
};
#ifdef GLOBAL_SINGLETON_REFERENCES
extern FSScreen& FSDraw;
#endif


#endif 
