#include "SpriteSetResource.h"
#include "SpriteResource.h"
#include "CanvasResource.h"

namespace FlatScene {

    template <class ImageType>
    ImageSet<ImageType>::ImageSet(std::string c) 
        : Handler (SpritesetResource<ImageType>::create(std::move(c)))
    {}

    template <class ImageType>
    const std::string& ImageSet<ImageType>::getName() const {
        return Handler::getRes().getName();
    }

    template <class ImageType>
    const std::vector<ImageType>& ImageSet<ImageType>::get() const {
        return Handler::getRes().get();
    }

    template <class ImageType>
    const std::vector<ImageType>* const ImageSet<ImageType>::operator->() const {
        return &Handler::getRes().get();
    }

    template class ImageSet<Sprite>;
    // template class ImageSet<Canvas>;
    // template class ImageSet<SoftwareSprite>;
    // template class ImageSet<SoftwareCanvas>;

} // flatscene
