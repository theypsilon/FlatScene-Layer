#ifndef FS_CANVAS_RES__
#define FS_CANVAS_RES__

#include "Canvas.h"
#include "RefCountMemoryPolicyImpl.h"
#include "GraphicResourceDescriptor.h"
#include "ImageId.h"
#include "ImageAdapter.h"

namespace FlatScene {

    class CanvasResource {
        typedef Canvas          Handler;

        CanvasResource (ImageId nid) : id(std::move(nid)) {}

        mutable GLuint      tex;
        Uint32              w, h;
        int                 w2,h2; /* previous value shifted to the very next bigger value which form is X^2 */
        Uint8               bpp;
        RawImageResource    raw;
        const ImageId       id;

        mutable std::list<std::function<void(void)>> initCallbackList;
        mutable std::list<std::function<void(void)>> endCallbackList;

        virtual ~CanvasResource(); // defined in ImageAdapter.h
        void cleanResourcesGPU();
        void cleanResourcesCPU();

        friend class Canvas;
        friend struct SRenderCanvas;
        friend class RefCountMemoryPolicy<CanvasResource>;
        friend class SpriteResource;
        friend struct ScreenImpl;

        template <class T> friend T* createResource(
            const RectangleImage& src, ConstRawImageResource chipset, GraphicMode mode,
            const GRD& grd, unsigned int n, GraphicFilter filter = NEAREST
        );

        template <typename PointType, typename GraphicMaterial>
        friend void putCanvas ( const PointType& ptDst, unsigned char flags, 
                                const CanvasResource& impl, GraphicMaterial& gm );

        friend void storeSurface(
            CanvasResource& canvas, RawImageResource surface, GraphicMode mode, GraphicFilter filter
        );

        friend void reloadSurface(CanvasResource& canvas);
    };


} // flatscene

#endif
