#ifndef __IMAGE_H__
#define __IMAGE_H__

#ifdef WIN32
#include <windows.h>
#endif

#include "GL/gl.h"          // Librer�a OpenGL32
#include "GL/glu.h"
#include "SDL.h"
#include "SDL_image.h"
#include "FSdefinitions.h"
#include "FSRectangle.h"
#include "FSPoint.h"
#include "FSColor.h"
#include <string>
#include <list>
#include <functional>
#include <map>

using namespace std;

typedef struct {
    GLuint tex;
    Uint32 w, h;                /* Read-only */
    int w2,h2;          /* Valor previo desplazado a la potencia de 2 superior o igual m�s pr�xima. */
    Uint8 bpp;
    SDL_Surface* sdl_surf; // NULL or not null, thats the question.
}SCanvas;

class FSCanvas {
private:
    SCanvas m_pSurface ;

    FSCanvas( SCanvas pSurface ) ;
    virtual ~FSCanvas( ) ;

    void clearSurface () ;

    friend class FSTextBox;
    friend class FSSprite;
    friend class FSSpriteset;
    friend class FSWriter;
    friend class FSImages;
    friend class FSScreen;

    static SCanvas toSCanvas ( SDL_Surface* , Uint8 mode=ONLY_TEXTURE, GLint filter=GL_NEAREST);
    static SDL_Surface* scaleSurface( SDL_Surface* s_surf,int factor);

    static inline Uint32 pow2 (Uint32 n);

	list<std::function<void(void)>> initCallbackList;
	list<std::function<void(void)>> endCallbackList;

public:

    SCanvas* getCanvas();

    // Funciona s�lo si hay SDL_Surface
    Uint32 getPixel ( int x , int y ) ;

    int getWidth ( ) ;
    int getHeight ( ) ;

    //render image
    void put ( FSPoint& ptDst , Uint8 flags=0) ;
    void put ( FSFloatPoint& ptDst , Uint8 flags=0) ;

    int rotate(float angle, float x=0.0, float y=0.0, float z=1.0);
    int translate(float x, float y, float z);
    int scale(float x, float y, float z);
    int color(float red, float green, float blue, float alpha);
    int color(FSColor* col,float alpha=1.0);
};

#endif
