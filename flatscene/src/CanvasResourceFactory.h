#ifndef FS_CANVAS_FACTORY__
#define FS_CANVAS_FACTORY__

#include "sdlSurfaceFuncs.h"
#include "SpriteResource.h"

namespace FlatScene {

    template <class Res, GraphicMode mode> 
    std::shared_ptr<Res> CanvasResource::create(
        const RectangleImage& src, ConstRawImageResource chipset, 
        const GRD& grd, unsigned int n, bool software
    ) {
        static_assert(
            std::is_base_of<CanvasResource,Res>::value,
            "Bad Canvas type"
        );

        ImageId id (n, grd.getDescriptorFile());

        auto ptr = std::static_pointer_cast<Res>(
            make_cached_shared<CanvasResource>(id,[&]{
                auto source = loadSurface(src,chipset,mode,grd.getSpecialScale());
                auto res = new Res(
                    id, 
                    BitmapHandler(software,source->pixels,source->w,source->h)
                );
                IMGFreeOrThrow(source);
                return res;
            })
        );

        cresSet.emplace(ptr);

        return ptr;





        // for (const auto& cres : cresSet) {
        //     auto shared = cres.second.lock();
        //     if (id == shared->id)
        //         return std::static_pointer_cast<Res>(shared);
        // }

        // auto source = loadSurface(src,chipset,mode,grd.getSpecialScale());

        // auto res = std::shared_ptr<Res>(new Res(
        //         std::move(id),
        //         BitmapHandler(software,source->pixels,source->w,source->h)
        //     ),[&](CanvasResource* p) {
        //         assert(cresSet.find(p)  != cresSet.end());
        //         assert(cresSet.find(p)->second.expired());
        //         cresSet.erase(p);
        //         delete p;
        //     }
        // );

        // cresSet.emplace(res.get(),std::static_pointer_cast<CanvasResource>(res));

        // IMGFreeOrThrow(source);
        
        // return res;
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