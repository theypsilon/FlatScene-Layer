#ifndef FS_CANVAS_FACTORY__
#define FS_CANVAS_FACTORY__

#include "sdlSurfaceFuncs.h"
#include "SpriteResource.h"

namespace FlatScene {

    template <class T> T* createResource(
        const SDL_Rect& src, const SDL_Surface& chipset, 
        GraphicMode mode, double sp_scale, GraphicFilter filter
    ) {
        static_assert(
            std::is_base_of<CanvasResource,T>::value,
            "Bad Canvas type"
        );

        Point p(src.x,src.y);

        for (const auto& pair : T::Handler::MemoryPolicyType::getCounts())
            if (pair.first->xy == p && pair.first->c == &chipset)
                return static_cast<T*>(pair.first);

        auto res = new T(p,&chipset);
        assert(res);

        storeSurface(
            *res,
            loadSurface(src,chipset,mode,sp_scale),
            mode,
            filter
        );
        
        return res;
    }

} // flatscene

#endif