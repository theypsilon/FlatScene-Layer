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

        static Cache<ImageId,CanvasResource> cache([](CanvasResource* p){
            gCRMonitor.erase(p);
            delete p;
        });

        ImageId id(n, grd.getDescriptorFile());

        auto ptr = cache.get(id,[&]{
            auto source = loadSurface(src,chipset,mode,grd.getSpecialScale());
            auto res = new Res(
                id, 
                BitmapHandler(software,source->pixels,source->w,source->h)
            );
            ScopedGuard guard([&]{ delete res; });
            IMGFreeOrThrow(source);
            gCRMonitor.insert(res);
            guard.dismiss();
            
            return res;
        });

        return std::static_pointer_cast<Res>(ptr);
    }

} // flatscene

#endif