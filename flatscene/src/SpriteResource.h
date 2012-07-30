#ifndef FS_SPRITE_RES__
#define FS_SPRITE_RES__

#include "Sprite.h"
#include "CanvasResource.h"
#include "Exception.h"

namespace FlatScene {

    class SpriteResource : public CanvasResource {
        typedef Sprite  Handler;

        std::string     name;
        Point           cpoint;
        Sprite::Areas   areas;
        SpriteOpaque    opaque;

        SpriteResource(const CanvasResource::PointType& xy,const SDL_Surface *const c) : CanvasResource(xy,c) {}
        virtual ~SpriteResource() {}

        friend class Sprite;
        template <class T> friend T* createResource(
            const SDL_Rect& src, const SDL_Surface& chipset, 
            GraphicMode mode, double sp_scale, GraphicFilter filter
        );
    };

} // flatscene
#endif
