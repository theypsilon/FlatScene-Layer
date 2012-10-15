#ifndef FS_CANVAS_RES__
#define FS_CANVAS_RES__

#include "Canvas.h"
#include "RefCountMemoryPolicyImpl.h"
#include "GraphicResourceDescriptor.h"
#include "ImageId.h"
#include "ImageAdapter.h"
#include "BitmapHandler.h"

namespace FlatScene {

    class CanvasResource {
    public:
        typedef Canvas          Handler;

        void                applyMetadata(const GRD::Image& img) {
            
        }

        template <class Res = CanvasResource, GraphicMode mode = ONLY_GPU> 
        static Res*             create(const RectangleImage& src, ConstRawImageResource chipset, 
                                       const GRD&  grd, unsigned int n       );

        template <class Res, class T1, class T2>
        static Res*             create(T1&& imageId, T2&& BitmapHandler);

        BitmapHandler::SizeType     getW() const       { return _gpu.getW();    }
        BitmapHandler::SizeType     getH() const       { return _gpu.getH();    }
        BitmapHandler::RelType      getRelW() const    { return _gpu.getRelW(); }
        BitmapHandler::RelType      getRelH() const    { return _gpu.getRelH(); }

        void                    put         (Float x, Float y, unsigned char flags) const;
        void                    translate   (Float x, Float y, Float z) const;
        void                    scale       (Float x, Float y, Float z) const;
        void                    rotate      (Float angle, Float x, Float y, Float z) const;
        void                    color       (Float red, Float green, Float blue, Float alpha) const;

        BitmapHandler::PixelType        getPixel(BitmapHandler::SizeType x, BitmapHandler::SizeType y) const 
                                        {   return _gpu.getPixel(x,y);  }
        const BitmapHandler::PAType&    getPixelBuffer() const
                                        {   return _gpu.getCPUBuffer(); }

        void    setPixel(BitmapHandler::SizeType x, BitmapHandler::SizeType y, BitmapHandler::PixelType p);

        void    modifyPixels(std::function<void(BitmapHandler::PAType&)> pred, bool flushchanges = true);
        void    replacePixels(const BitmapHandler::PAType& buf, bool flushchanges = true);

        virtual ~CanvasResource() {}

        const ImageId   id;
    private:
        template <class TImageId, class TBitmapHandler>
        CanvasResource (TImageId&& nid, TBitmapHandler&& gpu) 
            : id(std::forward<TImageId>(nid)), _gpu(std::forward<TBitmapHandler>(gpu)) {}

        BitmapHandler                                    _gpu;
        mutable std::list<std::function<void(void)>> _initCallbackList;
        mutable std::list<std::function<void(void)>> _endCallbackList;

        friend class SpriteResource;
        friend class ScreenImpl;
    };


} // flatscene

#endif
