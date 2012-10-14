#ifndef FS_IMAGE_H__
#define FS_IMAGE_H__

#include "Types.h"
#include "Color.h"
#include <string>
#include <list>
#include <functional>
#include <map>

#include "ResourceHandler.h"
#include "GraphicTypes.h"

namespace FlatScene {

    class CanvasResource;

    class Canvas 
    : public ResourceHandler<CanvasResource,RefCountMemoryPolicy<CanvasResource> > {
    public:
        int             getWidth() const;
        int             getHeight() const;

        unsigned int    getPixel(unsigned int x, unsigned int y) const;

        template <class PT>
        void            put(const Point2D<PT>& ptDst, unsigned char flags = 0) const 
                            { put((Float) ptDst.x, (Float) ptDst.y, flags); }
        void            put(Float x, Float y, unsigned char flags = 0) const;

        void            rotate(Float angle, Float x=0.0, Float y=0.0, Float z=1.0) const;
        void            translate(Float x, Float y, Float z) const;
        void            scale(Float x, Float y, Float z) const;
        void            color(Float red, Float green, Float blue, Float alpha) const;
        void            color(const Color& col,Float alpha=1.0) const;

    protected:
        Canvas(CanvasResource* res);
    private:
        
        friend class FSTextBox;
        friend class Sprite;
        template <class Res> friend class ImageSetResource;
        friend class Images;
        friend class Screen;
    };

    /* This class is the same as Canvas, but it can't be rendered. *
     * You may use this as a memory optimization                   */
    class SoftwareCanvas 
    : public ResourceHandler<CanvasResource,RefCountMemoryPolicy<CanvasResource> > {
    public:
        int             getWidth() const;
        int             getHeight() const;

        unsigned int    getPixel(unsigned int x, unsigned int y) const;
    protected:
        SoftwareCanvas(CanvasResource* res);
    private:
        
        friend class FSTextBox;
        friend class Sprite;
        template <class Res> friend class ImageSetResource;
        friend class Images;
        friend class Screen;
    };

    Canvas         modify(const Canvas& origin);
    SoftwareCanvas modify(const SoftwareCanvas& origin);

} // flatscene

#endif
