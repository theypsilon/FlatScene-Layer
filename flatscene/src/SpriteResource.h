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

        SpriteResource(ImageId id, BitmapGPU gpu) : CanvasResource(std::move(id), std::move(gpu)) {}
        virtual ~SpriteResource() {}

        friend class Sprite;
        friend class RefCountMemoryPolicy<SpriteResource>;
        template <class T> friend T* createResource(
            const RectangleImage& src, ConstRawImageResource chipset, GraphicMode   mode,
            const GRD& grd,  unsigned int n,             GraphicFilter filter
        );
    };

} // flatscene
#endif
