#ifndef FS_IMAGESET_H__
#define FS_IMAGESET_H__

#include "Sprite.h"

namespace FlatScene {

    template <class Res> class ImageSetResource;

    template <class ImageType>
    class ImageSet {
    public:
        ImageSet(std::string pathToFile);

        const std::string&                  getName   () const;
        const std::vector<ImageType>&       get       () const;
        const std::vector<ImageType>*const  operator->() const;
    private:
        std::shared_ptr<ImageSetResource<ImageType>> _res;
    };

    typedef ImageSet<Sprite>            Spriteset;
    typedef ImageSet<Canvas>            Canvasset;
    typedef ImageSet<SoftwareSprite>    SoftSpriteset;
    typedef ImageSet<SoftwareCanvas>    SoftCanvasset;

} // flatscene

#endif // FS_IMAGESET_H__
