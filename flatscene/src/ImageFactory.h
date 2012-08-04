#ifndef FS_CANVAS_FACTORY__
#define FS_CANVAS_FACTORY__

#include "sdlSurfaceFuncs.h"
#include "SpriteResource.h"

namespace FlatScene {

    template <class T> T* createResource(
        const RectangleImage& src, ConstRawImageResource chipset, GraphicMode    mode,
        const GRD&  grd, unsigned int       n      , GraphicFilter  filter
    ) {
        static_assert(
            std::is_base_of<CanvasResource,T>::value,
            "Bad Canvas type"
        );

        ImageId id (n, grd.getDescriptorFile());

        for (const auto& pair : T::Handler::MemoryPolicyType::getCounts())
            if (id == pair.first->id)
                return static_cast<T*>(pair.first);

        auto res = new T(std::move(id));
        assert(res);

        storeSurface(
            *res,
            loadSurface(src,chipset,mode,grd.getSpecialScale()),
            mode,
            filter
        );
        
        return res;
    }

} // flatscene

#endif