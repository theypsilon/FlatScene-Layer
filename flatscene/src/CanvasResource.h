#ifndef FS_CANVAS_RES__
#define FS_CANVAS_RES__

#include "Canvas.h"
#include "RefCountMemoryPolicyImpl.h"
#include "GraphicResourceDescriptor.h"
#include "ImageId.h"
#include "ImageAdapter.h"

namespace FlatScene {

    class CanvasResource {
    public:
        typedef Canvas          Handler;

        template <class Res = CanvasResource> 
        static Res*             create(const RectangleImage& src, ConstRawImageResource chipset, 
                                       GraphicMode mode, const GRD&  grd, unsigned int n       );

        template <class Res, class T1, class T2>
        static Res*             create(T1&& imageId, T2&& bitmapGPU);

        BitmapGPU::SizeType     getW() const       { return _gpu.getW();    }
        BitmapGPU::SizeType     getH() const       { return _gpu.getH();    }
        BitmapGPU::RelType      getRelW() const    { return _gpu.getRelW(); }
        BitmapGPU::RelType      getRelH() const    { return _gpu.getRelH(); }

        BitmapGPU::PixelType    getPixel(BitmapGPU::SizeType x, BitmapGPU::SizeType y) 
                                                {   return _gpu.getPixel(x,y);  }

        void                    put(Float x, Float y, unsigned char flags) const;
        void                    rotate(Float angle, Float x, Float y, Float z) const;
        void                    translate(Float x, Float y, Float z) const;
        void                    scale(Float x, Float y, Float z) const;
        void                    color(Float red, Float green, Float blue, Float alpha) const;

        virtual ~CanvasResource() {}

        const ImageId   id;
    private:
        template <class TImageId, class TBitmapGPU>
        CanvasResource (TImageId&& nid, TBitmapGPU&& gpu) 
            : id(std::forward<TImageId>(nid)), _gpu(std::forward<TBitmapGPU>(gpu)) {}

        BitmapGPU                                    _gpu;
        mutable std::list<std::function<void(void)>> _initCallbackList;
        mutable std::list<std::function<void(void)>> _endCallbackList;

        friend class Canvas;
        friend class RefCountMemoryPolicy<CanvasResource>;
        friend class SpriteResource;
        friend struct ScreenImpl;
    
    };


} // flatscene

#endif
