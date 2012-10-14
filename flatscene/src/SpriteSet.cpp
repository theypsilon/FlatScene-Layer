#include "SpriteSetResource.h"

namespace FlatScene {
    template <class ImageType>
    ImageSet<ImageType>::ImageSet(std::string c) 
        : ResourceHandler (SpritesetResource<ONLY_GPU>::create(std::move(c)))
    {}
    template <class ImageType>
    const std::string& ImageSet<ImageType>::getName() const {
        return getRes().getName();
    }
    template <class ImageType>
    const std::vector<ImageType>& ImageSet<ImageType>::get() const {
        return getRes().get();
    }
    template <class ImageType>
    const std::vector<ImageType>* const ImageSet<ImageType>::operator->() const {
        return &getRes().get();
    }

    template class ImageSet<Sprite>;
    // template class ImageSet<Canvas>;
    // template class ImageSet<SoftwareSprite>;
    // template class ImageSet<SoftwareCanvas>;

} // flatscene
