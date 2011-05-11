#ifndef __IMAGE_H__
#define __IMAGE_H__

#ifdef WIN32
#include <windows.h>
#endif

#include "GL/gl.h"			// Librería OpenGL32 
#include "GL/glu.h"
#include "SDL.h"
#include "SDL_image.h"
#include "FSdefinitions.h"
#include "FSRectangle.h"
#include "FSFloatPoint.h"
#include "FSColor.h"
#include <string>
#include <list>
#include <map>

using namespace std;

typedef struct {
	GLuint tex;
	Uint32 w, h;				/* Read-only */
	int w2,h2;			/* Valor previo desplazado a la potencia de 2 superior o igual más próxima. */
	Uint8 bpp;
	SDL_Surface* sdl_surf; // NULL or not null, thats the question.
	float sp_scale;
}SCanvas;

struct SToRender;

enum TypeResource {
	TR_CANVAS,
	TR_FLOATCANVAS,
	TR_ROTATION,
	TR_TRANSLATION,
	TR_LOCATION,
	TR_PUSHMATRIX,
	TR_POPMATRIX,
	TR_SCALATION,
	TR_COLOR
};

class CImage  
{
private:
	SCanvas m_pSurface ;

	CImage ( SCanvas pSurface ) ;
	virtual ~CImage ( ) ;

	void clearSurface () ;

	friend class CTextBox;
	friend class CSprite;
	friend class CSpriteset;
	friend class CControlOutputText;
	friend class CControlImages;
	friend class CScreen;

	static SCanvas toSCanvas ( SDL_Surface* , Uint8 mode=ONLY_TEXTURE, GLint filter = GL_NEAREST);
	static SDL_Surface* scaleSurface( SDL_Surface* s_surf, int factor=1);

	static inline Uint32 pow2 (Uint32 n);

	list<SToRender*> initRenderList;
	list<SToRender*> endRenderList;

	static map<TypeResource,void (*)(void*)> procRenders;

	static void initProcRenders();

	static void procRendRotation(void* pointer);
	static void procRendTranslation(void* pointer);
	static void procRendScalation(void* pointer);
	static void procRendColor(void* pointer);
	static void procRendPush(void* pointer);
	static void procRendPop(void* pointer);

public:

	SCanvas* getCanvas();
	// Funciona sólo si hay SDL_Surface
	Uint32 getPixel ( int x , int y ) ;

	int getWidth ( ) ;
	int getHeight ( ) ;

	//render image
	void put ( CPoint& ptDst , Uint8 flags=0) ;
	void put ( CFloatPoint& ptDst , Uint8 flags=0) ;

	int rotate(float angle, float x=0.0, float y=0.0, float z=1.0);
	int translate(float x, float y, float z);
	int scale(float x, float y, float z);
	int color(float red, float green, float blue, float alpha);
	int color(CColor* col,float alpha=1.0);
};

#endif
