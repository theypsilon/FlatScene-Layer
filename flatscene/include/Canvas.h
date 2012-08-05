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

    enum GraphicMode {  // Changes the memory used for containing the image data
        ONLY_CPU,       // -The image can not be rendered
        ONLY_GPU,       // -The pixel data can't be readed, getPixel now throws
        BOTH            // -No drawbacks in features, but it uses double memory
    };

    enum GraphicFilter {
        NEAREST,
        LINEAR
    };

    class CanvasResource;

    class Canvas : public ResourceHandler<CanvasResource,RefCountMemoryPolicy<CanvasResource> > {
    public:

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
        
        friend class FSTextBox;
        friend class Sprite;
        friend class SpritesetResource;
        friend class Images;
        friend class Screen;
    };

} // flatscene

#endif
