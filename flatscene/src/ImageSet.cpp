#include "ImageSetResource.h"
#include "SpriteResource.h"
#include "CanvasResource.h"

namespace FlatScene {

    template <class ImageType>
    ImageSet<ImageType>::ImageSet(std::string c) 
        : _res(ImageSetResource<ImageType>::create(std::move(c)))
    {}

    template <class ImageType>
    const std::string& ImageSet<ImageType>::getName() const {
        return _res->getName();
    }

    template <class ImageType>
    const std::vector<ImageType>& ImageSet<ImageType>::get() const {
        return _res->get();
    }

    template <class ImageType>
    const std::vector<ImageType>* const ImageSet<ImageType>::operator->() const {
        return &_res->get();
    }

    template class ImageSet<Sprite>;
    template class ImageSet<Canvas>;
    template class ImageSet<SoftwareSprite>;
    template class ImageSet<SoftwareCanvas>;

    std::unordered_map<std::string,std::weak_ptr<ImageSetResource<Sprite>>> setsInUse;
    // std::unordered_map<std::string,std::weak_ptr<ImageSetResource<Canvas>>> setsInUse;
    // std::unordered_map<std::string,std::weak_ptr<ImageSetResource<SoftwareSprite>>> setsInUse;
    // std::unordered_map<std::string,std::weak_ptr<ImageSetResource<SoftwareCanvas>>> setsInUse;

} // flatscene
