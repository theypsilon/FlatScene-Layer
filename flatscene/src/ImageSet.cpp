#include "ImageSetResource.h"
#include "SpriteResource.h"
#include "CanvasResource.h"

namespace FlatScene {

    namespace detail {
        template <class T> struct res_set { 
            typedef ResourceHandler<
                ImageSetResource<T>,
                RefCountMemoryPolicy<ImageSetResource<T> >
            > type;
        };
    } //detail

    template <class ImageType>
    ImageSet<ImageType>::ImageSet(std::string c) 
        : detail::res_set<ImageType>::type (ImageSetResource<ImageType>::create(std::move(c)))
    {}

    template <class ImageType>
    const std::string& ImageSet<ImageType>::getName() const {
        return detail::res_set<ImageType>::type::getRes().getName();
    }

    template <class ImageType>
    const std::vector<ImageType>& ImageSet<ImageType>::get() const {
        return detail::res_set<ImageType>::type::getRes().get();
    }

    template <class ImageType>
    const std::vector<ImageType>* const ImageSet<ImageType>::operator->() const {
        return &detail::res_set<ImageType>::type::getRes().get();
    }

    template class ImageSet<Sprite>;
    template class ImageSet<Canvas>;
    template class ImageSet<SoftwareSprite>;
    template class ImageSet<SoftwareCanvas>;

} // flatscene
