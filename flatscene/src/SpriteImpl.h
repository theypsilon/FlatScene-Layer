#ifndef FS_SPRITE_IMPL__
#define FS_SPRITE_IMPL__

#include "Sprite.h"
#include "CanvasResource.h"
#include "Exception.h"

namespace FlatScene {

    class SpriteResource : public CanvasResource {
    public:
        std::string     name;
        Point   		cpoint;
        Sprite::Areas   areas;
        SpriteOpaque    opaque;
    };

} // flatscene
#endif
