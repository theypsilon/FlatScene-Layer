#ifndef __IMAGE_H__
#define __IMAGE_H__

#ifdef WIN32
    #define NOMINMAX
    #include <windows.h>
#endif

#include "FSdefinitions.h"
#include "FSTypes.h"
#include "FSColor.h"
#include <string>
#include <list>
#include <functional>
#include <map>

#include "FSPimpl.h"

namespace flatscene {

    enum GraphicFilter {
        NEAREST,
        LINEAR
    };

    class Canvas {
    private:
        struct CanvasImpl;
        std::unique_ptr<CanvasImpl> _impl;

        Canvas ( const Canvas& ); //undefined
        Canvas() ;

        friend class FSTextBox;
        friend class Sprite;
        friend class Spriteset;
        friend class Writer;
        friend class Images;
        friend class Screen;

        static SDL_Surface* scaleSurface( SDL_Surface* s_surf,int factor);

    public:
        ~Canvas( ) ;

    public:
        Canvas( Canvas&& pSurface ) ;

        template <class T=Canvas> static T createCanvas(SDL_Surface* surface, Uint8 mode=ONLY_TEXTURE, GraphicFilter filter=NEAREST);
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
