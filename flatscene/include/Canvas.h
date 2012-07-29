#ifndef FS_IMAGE_H__
#define FS_IMAGE_H__

#include "Types.h"
#include "Color.h"
#include <string>
#include <list>
#include <functional>
#include <map>

#include "ResourceHandler.h"

namespace FlatScene {

    enum GraphicMode {
        ONLY_TEXTURE,
        WITH_SDL_SURFACE,
        ONLY_SDL_SURFACE
    };

    enum GraphicFilter {
        NEAREST,
        LINEAR
    };

    class CanvasResource;

    class Canvas : public ResourceHandler<CanvasResource,RefCountMemoryPolicy<CanvasResource> > {
    public:
        typedef RefCountMemoryPolicy<CanvasResource>                MemoryPolicyType;
        typedef ResourceHandler<CanvasResource,MemoryPolicyType>    ResourceHandlerType;

        Canvas(const Point& xy, const SDL_Surface *const c);    

        unsigned int getPixel(unsigned int x, unsigned int y) const;

        int getWidth() const;
        int getHeight() const;

        //render image
        void put(const Point& ptDst, unsigned char flags=0) const;
        void put(const FloatPoint& ptDst, unsigned char flags=0) const;

        void rotate(Float angle, Float x=0.0, Float y=0.0, Float z=1.0) const;
        void translate(Float x, Float y, Float z) const;
        void scale(Float x, Float y, Float z) const;
        void color(Float red, Float green, Float blue, Float alpha) const;
        void color(const Color& col,Float alpha=1.0) const;

    protected:
        Canvas(CanvasResource* res);
    private:
        template <typename T> friend T createCanvas(
            const SDL_Rect& src, const SDL_Surface& chipset, 
            GraphicMode mode, double sp_scale, GraphicFilter filter=NEAREST
        );
        
        friend class FSTextBox;
        friend class Sprite;
        friend class SpritesetResource;
        friend class Images;
        friend class Screen;
    };

} // flatscene

#endif
