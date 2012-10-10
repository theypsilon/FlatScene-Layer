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

        template <class TImageId, class TBitmapGPU>
        CanvasResource (TImageId&& nid, TBitmapGPU&& gpu) 
            : id(std::forward<TImageId>(nid)), _gpu(std::forward<TBitmapGPU>(gpu)) {}

        BitmapGPU       _gpu;
        const ImageId   id;

        BitmapGPU::SizeType  getW() const    { return _gpu.getW();    };
        BitmapGPU::SizeType  getH() const    { return _gpu.getH();    };
        BitmapGPU::RelType   getRelW() const { return _gpu.getRelW(); };
        BitmapGPU::RelType   getRelH() const { return _gpu.getRelH(); };

        BitmapGPU::PixelType getPixel(BitmapGPU::SizeType x, BitmapGPU::SizeType y) {
            return _gpu.getPixel(x,y);
        }

        mutable std::list<std::function<void(void)>> initCallbackList;
        mutable std::list<std::function<void(void)>> endCallbackList;

        virtual ~CanvasResource(); // defined in ImageAdapter.h
        void cleanResourcesGPU();
        void cleanResourcesCPU();

        //render image
        void put(const Point& ptDst, unsigned char flags=0) const;
        void put(const FloatPoint& ptDst, unsigned char flags=0) const;

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

        friend void reloadResourcesGPU(CanvasResource& canvas);
    };


} // flatscene

#endif
