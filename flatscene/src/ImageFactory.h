#ifndef FS_CANVAS_FACTORY__
#define FS_CANVAS_FACTORY__

#include "sdlSurfaceFuncs.h"
#include "SpriteResource.h"

namespace FlatScene {

    template <class Res> Res* createResource(
        const RectangleImage& src, ConstRawImageResource chipset, GraphicMode    mode,
        const GRD&  grd, unsigned int       n      , GraphicFilter  filter
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

        auto res = new Res(std::move(id),BitmapGPU(source->pixels,source->w,source->h));
        assert(res);

        IMGFreeOrThrow(source);
        
        return res;
    }

    template <class Res, class T1, class T2>
    Res* createResource(T1&& imageId, T2&& bitmapGPU) {
        static_assert(std::is_base_of<CanvasResource,Res>::value &&
                      std::is_same   <ImageId  ,T1      >::value &&
                      std::is_same   <BitmapGPU,T2      >::value,
                      "Res, T1 and/or T2 are not acceptable types (check impl)"
        );

        return new Res(std::forward<T1>(imageId),std::forward<T2>(bitmapGPU));
    }

} // flatscene

#endif