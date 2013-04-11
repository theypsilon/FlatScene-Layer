#ifndef FS_CANVAS_H__
#define FS_CANVAS_H__

#include "Types.h"
#include "Color.h"
#include "GraphicTypes.h"

#include <memory>

namespace FlatScene {

    class CanvasResource;

    class Canvas {
    public:
        typedef CanvasResource ResourceType;

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
        Canvas(std::shared_ptr<CanvasResource> res) : _res(res) {}
        std::shared_ptr<CanvasResource> _res;
    private:
        
        template <class Res> friend class ImageSetResource;
        friend class Screen;
    };

    /* This class is the same as Canvas, but it can't be rendered. *
     * You may use this as a memory optimization                   */
    class SoftwareCanvas {
    public:
        typedef CanvasResource ResourceType;
        
        int             getWidth() const;
        int             getHeight() const;

        unsigned int    getPixel(unsigned int x, unsigned int y) const;
    protected:
        SoftwareCanvas(std::shared_ptr<CanvasResource> res) : _res(res) {}
        std::shared_ptr<CanvasResource> _res;
    private:
        
        template <class Res> friend class ImageSetResource;
        friend class Screen;
    };

    Canvas         modify(const Canvas& origin);
    SoftwareCanvas modify(const SoftwareCanvas& origin);

} // flatscene

#endif
