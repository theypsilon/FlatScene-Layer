#ifndef FS__SPRITESET_H__
#define FS__SPRITESET_H__

#include "Sprite.h"
#include "ResourceHandler.h"

namespace FlatScene {

    template <class Res> class ImageSetResource;

    template <class ImageType>
    class ImageSet 
    : public ResourceHandler<
        ImageSetResource<ImageType>,
        RefCountMemoryPolicy<ImageSetResource<ImageType>> > {
    public:
        ImageSet(std::string pathToLoadTheFile);

        const std::string&                  getName   () const;
        const std::vector<ImageType>&       get       () const;
        const std::vector<ImageType>*const  operator->() const;
    };

    typedef ImageSet<Sprite>            Spriteset;
    typedef ImageSet<Canvas>            Canvasset;
    typedef ImageSet<SoftwareSprite>    SoftSpriteset;
    typedef ImageSet<SoftwareCanvas>    SoftCanvasset;

} // flatscene

#endif // FS__SPRITESET_H__
