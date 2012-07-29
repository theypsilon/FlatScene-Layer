#ifndef FS_CANVAS_FACTORY__
#define FS_CANVAS_FACTORY__

#include "sdlSurfaceFuncs.h"
#include "CanvasResource.h"

namespace FlatScene {

    template <class T> T createCanvas(
        const SDL_Rect& src, const SDL_Surface& chipset, 
        GraphicMode mode, double sp_scale, GraphicFilter filter
    ) {
        static_assert(
            /*std::is_trivially_constructible<T>::value && */
            std::is_base_of<Canvas,T>::value,
            "Bad Canvas type"
        );

        Point p(src.x,src.y);

        for (const auto& pair : T::MemoryPolicyType::getCounts())
            if (pair.first->xy == p && pair.first->c == &chipset)
                return T(pair.first);

        T newCanvas(p,&chipset);

        storeSurface(
            static_cast<Canvas&>(newCanvas).getRes(),
            loadSurface(src,chipset,mode,sp_scale),
            mode,
            filter
        );
        
        return newCanvas;
    }

} // flatscene

#endif