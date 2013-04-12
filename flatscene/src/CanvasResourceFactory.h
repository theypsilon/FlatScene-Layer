#ifndef FS_CANVAS_FACTORY__
#define FS_CANVAS_FACTORY__

#include "sdlSurfaceFuncs.h"
#include "SpriteResource.h"
#include "ScopedGuard.h"

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

        auto ptr = make_cached_shared<CanvasResource>(id,[&]{
            auto source = loadSurface(src,chipset,mode,grd.getSpecialScale());
            auto res = new Res(
                id, 
                BitmapHandler(software,source->pixels,source->w,source->h)
            );
            ScopedGuard guard([&]{
                delete res;
            });
            IMGFreeOrThrow(source);
            guard.dismiss();
            return res;
        });

        gCRMonitor.emplace(ptr);

        return std::static_pointer_cast<Res>(ptr);
    }

} // flatscene

#endif