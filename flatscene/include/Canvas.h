#ifndef FS_IMAGE_H__
#define FS_IMAGE_H__

#include "definitions.h"
#include "Types.h"
#include "Color.h"
#include <string>
#include <list>
#include <functional>
#include <map>

#include "Pimpl.h"

namespace FlatScene {

    enum GraphicFilter {
        NEAREST,
        LINEAR
    };

    class CanvasResource;

    class Canvas {
    public:

        Canvas(Canvas&& pSurface);
        ~Canvas() ;

        template <typename T> 
        static T createCanvas(SDL_Surface* surface, unsigned char mode=ONLY_TEXTURE, GraphicFilter filter=NEAREST);
        // Funciona s�lo si hay SDL_Surface
        unsigned int getPixel ( int x , int y ) const;

        int getWidth ( ) const;
        int getHeight ( ) const;

        //render image
        void put ( const Point& ptDst , unsigned char flags=0) const;
        void put ( const FloatPoint& ptDst , unsigned char flags=0) const;

        void rotate(Float angle, Float x=0.0, Float y=0.0, Float z=1.0) const;
        void translate(Float x, Float y, Float z) const;
        void scale(Float x, Float y, Float z) const;
        void color(Float red, Float green, Float blue, Float alpha) const;
        void color(const Color& col,Float alpha=1.0) const;

    protected:
        Canvas(CanvasResource* res);
        template <typename Resource = CanvasResource> inline Resource& getRes() const {
            return *static_cast<Resource*>(_res.get());
        }
    private:
        std::unique_ptr<CanvasResource> _res;

        Canvas ( const Canvas& ); //undefined
        Canvas() ;

        friend class FSTextBox;
        friend class Sprite;
        friend class Spriteset;
        friend class Images;
        friend class Screen;

        static SDL_Surface* scaleSurface( SDL_Surface* s_surf,int factor);
    };

} // flatscene

#endif
