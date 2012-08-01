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

        GLuint              tex;
        Uint32              w, h;                /* Read-only */
        int                 w2,h2;          /* Valor previo desplazado a la potencia de 2 superior o igual m�s pr�xima. */
        Uint8               bpp;
        RawImageResource    raw; // NULL or not null, thats the question.
        const ImageId       id;

        void clearSurface () {
            bpp = h = h2 = w = w2 = tex = 0;
            raw = nullptr;
        }

        mutable std::list<std::function<void(void)>> initCallbackList;
        mutable std::list<std::function<void(void)>> endCallbackList;

        virtual ~CanvasResource(); // defined in ImageAdapter.h

        friend class Canvas;
        friend struct SRenderCanvas;
        friend class RefCountMemoryPolicy<CanvasResource>;
        friend class SpriteResource;

        template <class T> friend T* createResource(
            const SDL_Rect& src, ConstRawImageResource chipset, GraphicMode mode,
            const DataGRD& grd, unsigned int n, GraphicFilter filter = NEAREST
        );

        template <typename PointType, typename GraphicMaterial>
        friend void putCanvas ( const PointType& ptDst, unsigned char flags, 
                                const CanvasResource& impl, GraphicMaterial& gm );

        friend void storeSurface(
            CanvasResource& canvas, RawImageResource surface, GraphicMode mode, GraphicFilter filter
        );
    };


} // flatscene

#endif
