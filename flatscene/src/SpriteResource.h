#ifndef FS_SPRITE_RES__
#define FS_SPRITE_RES__

#include "Sprite.h"
#include "CanvasResource.h"
#include "Exception.h"

namespace FlatScene {

    class SpriteResource : public CanvasResource {
    public:
        std::string     name;
        Point           cpoint;
        Sprite::Areas   areas;
        SpriteOpaque    opaque;

        SpriteResource(const CanvasResource::PointType& xy,const SDL_Surface *const c) : CanvasResource(xy,c) {}
        virtual ~SpriteResource() {}
    };

} // flatscene
#endif
