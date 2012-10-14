#ifndef FS__SPRITESET_H__
#define FS__SPRITESET_H__

#include "Sprite.h"
#include "ResourceHandler.h"

namespace FlatScene {

    template <class Res> class SpritesetResource;

    template <class ImageType>
    class ImageSet 
    : public ResourceHandler<
        SpritesetResource<ImageType>,
        RefCountMemoryPolicy<SpritesetResource<ImageType>> > {
        typedef ResourceHandler<
            SpritesetResource<ImageType>,
            RefCountMemoryPolicy<SpritesetResource<ImageType> >
        > Handler;
    public:
        ImageSet(std::string c    = "");

        const std::string&         getName() const;
        const std::vector<ImageType>& get    () const;
        const std::vector<ImageType>* const operator->() const;
    };

    typedef ImageSet<Sprite> Spriteset;
    typedef ImageSet<Canvas> Canvasset;
    typedef ImageSet<SoftwareSprite> SoftSpriteset;
    typedef ImageSet<SoftwareCanvas> SoftCanvasset;

} // flatscene

#endif // FS__SPRITESET_H__
