#ifndef FS_CANVAS_FACTORY__
#define FS_CANVAS_FACTORY__

#include "sdlSurfaceFuncs.h"
#include "SpriteResource.h"

namespace FlatScene {

    template <class Res, GraphicMode mode> Res* CanvasResource::create(
        const RectangleImage& src, ConstRawImageResource chipset, 
        const GRD& grd, unsigned int n
    ) {
        static_assert(
            std::is_base_of<CanvasResource,Res>::value,
            "Bad Canvas type"
        );

        ImageId id (n, grd.getDescriptorFile());

        for (const auto& pair : Res::Handler::MemoryPolicyType::getCounts())
            if (id == pair.first->id)
                return static_cast<Res*>(pair.first);

        auto source = loadSurface(src,chipset,mode,grd.getSpecialScale());

        auto res = new Res(std::move(id),BitmapHandler(source->pixels,source->w,source->h));
        assert(res);

        IMGFreeOrThrow(source);
        
        return res;
    }

    // template <class Res, class T1, class T2>
    // Res* CanvasResource::create(T1&& imageId, T2&& BitmapHandler) {
    //     static_assert(std::is_base_of<CanvasResource,Res>::value &&
    //                   std::is_same   <ImageId  ,T1      >::value &&
    //                   std::is_same   <BitmapHandler,T2      >::value,
    //                   "Res, T1 and/or T2 are not acceptable types (check impl)"
    //     );

    //     return new Res(std::forward<T1>(imageId),std::forward<T2>(BitmapHandler));
    // }

} // flatscene

#endif