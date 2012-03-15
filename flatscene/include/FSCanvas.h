#ifndef __IMAGE_H__
#define __IMAGE_H__

#ifdef WIN32
    #define NOMINMAX
    #include <windows.h>
#endif

#include "GL/gl.h"          // Librer�a OpenGL32
#include "GL/glu.h"
#include "SDL.h"
#include "SDL_image.h"
#include "FSdefinitions.h"
#include "FSTypes.h"
#include "FSColor.h"
#include <string>
#include <list>
#include <functional>
#include <map>

namespace flatscene {

    typedef struct {
        GLuint tex;
        Uint32 w, h;                /* Read-only */
        int w2,h2;          /* Valor previo desplazado a la potencia de 2 superior o igual m�s pr�xima. */
        Uint8 bpp;
        SDL_Surface* sdl_surf; // NULL or not null, thats the question.
    }SCanvas;

    class Canvas {
    private:
        GLuint tex;
        Uint32 w, h;                /* Read-only */
        int w2,h2;          /* Valor previo desplazado a la potencia de 2 superior o igual m�s pr�xima. */
        Uint8 bpp;
        SDL_Surface* sdl_surf; // NULL or not null, thats the question.

        Canvas ( const Canvas& ); //undefined

        Canvas() ;
        Canvas( const SCanvas& canvas ) ;
    
    

        void clearSurface () ;

        friend class FSTextBox;
        friend class Sprite;
        friend class Spriteset;
        friend class Writer;
        friend class Images;
        friend class Screen;

        static SDL_Surface* scaleSurface( SDL_Surface* s_surf,int factor);

        static inline Uint32 pow2 (Uint32 n);

        mutable std::list<std::function<void(void)>> initCallbackList;
        mutable std::list<std::function<void(void)>> endCallbackList;

    public:
        ~Canvas( ) ;

    public:
        Canvas( Canvas&& pSurface ) ;

        static Canvas toSCanvas ( SDL_Surface* , Uint8 mode=ONLY_TEXTURE, GLint filter=GL_NEAREST);
        template <class T> static T createCanvas(SDL_Surface* surface, Uint8 mode=ONLY_TEXTURE, GLint filter=GL_NEAREST);
        // Funciona s�lo si hay SDL_Surface
        Uint32 getPixel ( int x , int y ) const;

        int getWidth ( ) const;
        int getHeight ( ) const;

        //render image
        void put ( const Point& ptDst , Uint8 flags=0) const;
        void put ( const FloatPoint& ptDst , Uint8 flags=0) const;

        int rotate(Float angle, Float x=0.0, Float y=0.0, Float z=1.0) const;
        int translate(Float x, Float y, Float z) const;
        int scale(Float x, Float y, Float z) const;
        int color(Float red, Float green, Float blue, Float alpha) const;
        int color(Color* col,Float alpha=1.0) const;
    };

} // flatscene

#endif
