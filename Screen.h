#ifndef __MAINCANVAS_H__
#define __MAINCANVAS_H__

#include "Image.h"
#include "ControlImages.h"
#include "Color.h"
#include <list>

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
	CPoint ptDst;
	Uint8 flags;

	SRenderCanvas(SCanvas canvas, CPoint ptDst, Uint8 flags) 
	: canvas(canvas), ptDst(ptDst), flags(flags) {
	}
};

struct SRenderFloatCanvas {
	SCanvas canvas;
	CFloatPoint ptDst;
	Uint8 flags;

	SRenderFloatCanvas(SCanvas canvas,CFloatPoint ptDst, Uint8 flags) 
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



//main canvas, abstract primary display surface
class CScreen
{
private:
	static SDL_Surface* m_SDL_Surface;

	static bool rendering;
	static TypeRendeProjection trp;

	static float m_maxZ;

	static bool m_FullScreen,m_Doublebuff;

	static int m_Width,m_Height,m_Bpp;

	static float m_ScaleX,m_ScaleY;

	static list<SToRender*> graphicMaterial;

	static list<CSprite*> spriteToDelete;
	static list<CSpriteset*> spritesetToDelete;
	static list<CImage*> imageToDelete;

	static void deleteResources();

	static void saveResources(GraphicResources& info);
	static void reloadResources(GraphicResources& info);


	static float red,green,blue,alpha;


	static map<TypeResource,void (*)(void*)> procRenders;

	static void initProcRenders();

	static void procRendCanvas(void* pointer);
	static void procRendFloatCanvas(void* pointer);
	static void procRendRotation(void* pointer);
	static void procRendTranslation(void* pointer);
	static void procRendLocation(void* pointer);
	static void procRendPush(void* pointer);
	static void procRendPop(void* pointer);
	static void procRendScalation(void* pointer);
	static void procRendColor(void* pointer);

	static int beginRenderMode(Uint32 flags);
	static int endRenderMode(Uint32 flags);

	friend class CLibrary;
	friend class CCamera;

	friend class CSpriteset;
	friend class CSprite;
	friend class CImage;

	friend class CControlImages;
	friend class CControlOutputText;
	friend class CTextBox;

public:
	//constructor
	static int start ( int width , int height , int bpp , bool fullscreen, bool doublebuff=true ) ;
	static int start ( int width , int height , int bpp , float scalex, float scaley,bool fullscreen, bool doublebuff=true ) ;

	static int render();

	static int clear ( ) ;

	static int rotate(float angle, float x=0.0, float y=0.0, float z=1.0);
	static int translate(float x, float y, float z);
	static int scale(float x, float y, float z);
	static int color(float red, float green, float blue, float alpha);
	static int color(CColor* col,float alpha=1.0);
	static int projectionMode(TypeRendeProjection trp, float zMax=400.0);

	static int pushMatrix();
	static int popMatrix();

	static int locateRenderScene ( float posx=0.0, float posy=0.0, float width=m_Width, float height=m_Height, float zoom = 1.0) ;

	static float getScaleX();
	static float getScaleY();

	static Uint8 getBpp();

	static  int changeScreen( int width , int height , int bpp ,  float scalex=1.0, float scaley=1.0, bool fullscreen = m_FullScreen) ;

	static int ToggleFullscreen ();

	static int setDoublebuffer (bool doublebuff);
	//destructor
	static int quit();

	static int getWidth() { return m_Width; };

	static int getHeight() { return m_Height; };

	static bool isFullscreen() { return m_FullScreen; };

	static float getR(),getG(),getB(),getA();
};

#endif 
