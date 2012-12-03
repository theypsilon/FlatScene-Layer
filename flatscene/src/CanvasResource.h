#ifndef FS_CANVAS_RES__
#define FS_CANVAS_RES__

#include "Canvas.h"
#include "RefCountMemoryPolicyImpl.h"
#include "GraphicResourceDescriptor.h"
#include "ImageId.h"
#include "ImageAdapter.h"
#include "BitmapHandler.h"

#include <unordered_map>

namespace FlatScene {

    typedef std::weak_ptr<CanvasResource> SetCResElement;
    typedef std::unordered_map<CanvasResource*,SetCResElement> SetCRes;

    extern SetCRes cresSet;

    class CanvasResource {
    public:
        typedef Canvas          Handler;

        void                applyMetadata(const GRD::Image& img) {
            
        }

        template <class Res, GraphicMode mode> 
        static std::shared_ptr<Res> create(const RectangleImage& src, ConstRawImageResource chipset, 
                                       const GRD&  grd, unsigned int n, bool software = false );

        template <class Res, class T1, class T2>
        static Res*                 create(T1&& imageId, T2&& BitmapHandler);

        BitmapHandler::SizeType     getW() const       { return _gpu.inGPU()? _gpu.getTexW() : _gpu.getW(); } //TODO this shouldn't be necessary, hidden bug in rendering?
        BitmapHandler::SizeType     getH() const       { return _gpu.inGPU()? _gpu.getTexH() : _gpu.getH(); }
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
                                        {   if (!_gpu.inCPU()) _gpu.copyToCPU(); return _gpu.getCPUBuffer(); }

        void    setPixel(BitmapHandler::SizeType x, BitmapHandler::SizeType y, BitmapHandler::PixelType p);

        void    modifyPixels(std::function<void(BitmapHandler::PAType&)> pred, bool flushchanges = true);
        void    replacePixels(const BitmapHandler::PAType& buf, bool flushchanges = true);

        virtual ~CanvasResource() {}

        template <class TImageId, class TBitmapHandler>
        CanvasResource (TImageId&& nid, TBitmapHandler&& gpu) 
            : id(std::forward<TImageId>(nid)), _gpu(std::forward<TBitmapHandler>(gpu)) {}

        const ImageId   id;
    private:

        BitmapHandler                                    _gpu;
        mutable std::list<std::function<void(void)>> _initCallbackList;
        mutable std::list<std::function<void(void)>> _endCallbackList;

        friend class SpriteResource;
        friend struct ScreenImpl;
    };


} // flatscene

#endif
